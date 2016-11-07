#include "MeshFactory.hpp"

#include <iostream>

#include "assimp-3.0/include/assimp/Importer.hpp"
#include "assimp-3.0/include/assimp/scene.h"
#include "assimp-3.0/include/assimp/anim.h"
#include "assimp-3.0/include/assimp/mesh.h"
#include "assimp-3.0/include/assimp/types.h"
#include "assimp-3.0/include/assimp/postprocess.h"

#include "Logger.hpp"
#include "Constants.hpp"


namespace ys_render_machine {


// NOTE: This function actually does everything needed to render a mesh.
//		 Produces a Mesh, its associated node(s), a VertexArray, adds everything
//		 to the scene.
void
MeshFactory::LoadIntoScene(Scene& _scene, const std::string& _file)
{
	Logger::Log("Loading Nodes..", Logger::kChannelStdOut);

	Assimp::Importer	main_importer;

	const aiScene*		assimp_scene =
		main_importer.ReadFile(_file, 0u);//aiProcess_Triangulate);
	if (!assimp_scene)
		Logger::Log(main_importer.GetErrorString(), Logger::kChannelMeshFactory);

	Node*					scene_root = _scene.root();
	std::vector<Mesh*>&		ys_meshes = _scene.meshes();

	// THIS IS GOING TO BE SOME HEAVY PREPROCESSING
	// First we should make the Meshes. 
	// Then, when going through the Assimp scene hierarchy, we will be able to 
	// get the mat4* corresponding to our bones.
	
	Logger::Log("Mesh count : " + std::to_string(assimp_scene->mNumMeshes),
				Logger::kChannelMeshFactory);
	for (unsigned int i = 0; i < assimp_scene->mNumMeshes; ++i)
	{
		Logger::Log("============================", 
					Logger::kChannelMeshFactory);

		// For each mesh provided by Assimp we produce the matching internal structure.
		aiMesh*		assimp_mesh = assimp_scene->mMeshes[i];
		Mesh*		ys_mesh = new Mesh();
		ys_mesh->bounds[0] = ys_mesh->bounds[1] = vec4();

		// We add any bone data the current mesh might reference.
		// Structure hierarchy : 
		// Mesh 1->* Bone 1->* Weight
		Logger::Log("BONES", Logger::kChannelMeshFactory);
		for (unsigned int bone_index = 0;
			 bone_index < assimp_mesh->mNumBones; ++bone_index)
		{
			aiBone*		assimp_bone = assimp_mesh->mBones[bone_index];
			Mesh::Bone	ys_bone;

			Logger::Log("Bone : " + std::to_string(bone_index) + " " + 
						assimp_bone->mName.C_Str(), 
						Logger::kChannelMeshFactory);
			
			for (unsigned int weight_index = 0;
				 weight_index < assimp_bone->mNumWeights; ++weight_index)
			{
				aiVertexWeight	assimp_weight;
				assimp_weight = assimp_bone->mWeights[weight_index];

				ys_bone.weight_map.emplace(assimp_weight.mVertexId,
										   assimp_weight.mWeight);
				
				Logger::Log(std::to_string(assimp_weight.mWeight) + " ; " +
							std::to_string(assimp_weight.mVertexId), 
							Logger::kChannelMeshFactory);
			}

			ys_mesh->bones.emplace(assimp_bone->mName.C_Str(), ys_bone);
			Logger::Log("", Logger::kChannelMeshFactory);
		}

		// Once the bones were added, we also add our vertices.
		Logger::Log("VERTICES", Logger::kChannelMeshFactory);
		for (unsigned int vertex_index = 0;
			 vertex_index < assimp_mesh->mNumVertices; ++vertex_index)
		{
			aiVector3D	assimp_vertex = assimp_mesh->mVertices[vertex_index];
			vec4		ys_vertex(assimp_vertex.x, 
								  assimp_vertex.y, 
								  assimp_vertex.z);

			ys_mesh->vertices.push_back(ys_vertex);

			// BOUNDS UPDATE
			for (int i = 0; i < 3; ++i)
			{
				if (ys_vertex.E[i] < ys_mesh->bounds[0].E[i])
					ys_mesh->bounds[0].E[i] = ys_vertex.E[i];// min
				if (ys_vertex.E[i] > ys_mesh->bounds[1].E[i])
					ys_mesh->bounds[1].E[i] = ys_vertex.E[i]; // max
			}


			Logger::Log(std::to_string(vertex_index) + " " +
						std::to_string(ys_vertex.x) + " " + 
						std::to_string(ys_vertex.y) + " " + 
						std::to_string(ys_vertex.z), 
						Logger::kChannelMeshFactory);
		}

		Logger::Log("INDICES", Logger::kChannelMeshFactory);
		for (unsigned int face_index = 0;
			 face_index < assimp_mesh->mNumFaces; ++face_index)
		{
			std::string msg = std::to_string(face_index) + " ";
			aiFace		assimp_face = assimp_mesh->mFaces[face_index];
			
			for (unsigned int i = 0; i < assimp_face.mNumIndices; ++i)
			{
				if (assimp_face.mNumIndices != 3)
					Logger::Log("Non triangle face found", Logger::kChannelMeshFactory);

				msg += std::to_string(assimp_face.mIndices[i]) + " ";
				ys_mesh->indices.push_back(assimp_face.mIndices[i]);
			}

			Logger::Log(msg, Logger::kChannelMeshFactory);
		}

		ys_meshes.push_back(ys_mesh);

		VertexArray* vert_array = new VertexArray();
		vert_array->BindMesh(ys_mesh);
		_scene.vertex_arrays().push_back(vert_array);
	}


	aiNode*					root = assimp_scene->mRootNode;
	// We create two stacks that will be scanned the same way.
	std::vector<aiNode*>	assimp_stack;
	std::vector<Node*>		ys_stack;

	// Start things off with pushing roots back.
	assimp_stack.push_back(root);

	Node*	ys_mesh_root = new Node(root->mName.C_Str());
	scene_root->AddChild(ys_mesh_root);
	ys_stack.push_back(ys_mesh_root);

	while (assimp_stack.size())
	{
		aiNode*		assimp_node = assimp_stack.back();
		Node*		ys_node = ys_stack.back();
		assimp_stack.pop_back();
		ys_stack.pop_back();

		// Retrieving the Assimp node transform.
		// Assimp seems to store matrices in a row major format.
		aiMatrix4x4		intermediate_matrix = assimp_node->mTransformation;
		intermediate_matrix = intermediate_matrix.Transpose();
		memcpy(ys_node->transform().data,
			   &intermediate_matrix.a1,
			   sizeof(float) * 16);

		// We find out if the current node is referenced by a bone.
		for (std::vector<Mesh*>::iterator ite = ys_meshes.begin();
			 ite != ys_meshes.end(); ++ite)
		{
			std::unordered_map<std::string, Mesh::Bone>::iterator bone_ite;
			bone_ite = (*ite)->bones.find(ys_node->name());

			if (bone_ite != (*ite)->bones.end())
			{
				// Our bone structures stores a pointer to a matrix.
				bone_ite->second.node = ys_node;
			}
		}

		// Every mesh attached to the current node will get a pointer to its
		// transform
		for (unsigned int i = 0; i < assimp_node->mNumMeshes; ++i)
		{
			ys_meshes[assimp_node->mMeshes[i]]->node =
				ys_node;

			//ys_node->transform() = mat4::Scale(0.0001f) * ys_node->transform();
		}

		// Once we have correctly fetched the current node data, we can add its
		// children to our stack so that they can be processed later on.
		for (unsigned int i = 0; i < assimp_node->mNumChildren; ++i)
		{
			assimp_stack.push_back(assimp_node->mChildren[i]);
			
			Node*	child_node = new Node(assimp_node->mChildren[i]->mName.C_Str());
			ys_node->AddChild(child_node);
			ys_stack.push_back(child_node);
		}
	}
}


void
MeshFactory::DebugLoadArrays(Scene& _scene, const std::string& _name,
							 float* _vertices, int _stride, int _vsize,
							 uint32_t* _indices, int _isize)
{
	Mesh* ys_mesh = new Mesh();
	ys_mesh->node = new Node(_name);
	ys_mesh->bounds[0] = ys_mesh->bounds[1] = vec4();

	int vcount = _vsize / _stride;
	for (int i = 0; i < vcount; ++i)
	{
		vec4 ys_vertex = vec4();
		for (int j = 0; j < _stride; ++j)
		{
			ys_vertex.E[j] = *(_vertices + i * _stride + j);

			if (ys_vertex.E[i] < ys_mesh->bounds[0].E[i])
				ys_mesh->bounds[0].E[i] = ys_vertex.E[i];// min
			if (ys_vertex.E[i] > ys_mesh->bounds[1].E[i])
				ys_mesh->bounds[1].E[i] = ys_vertex.E[i]; // max
		}
		ys_mesh->vertices.push_back(ys_vertex);
	}

	for (int i = 0; i < _isize; ++i)
	{
		ys_mesh->indices.push_back(*(_indices + _isize));
	}

	_scene.root()->AddChild(ys_mesh->node);
	_scene.meshes().push_back(ys_mesh);
	VertexArray* vert_array = new VertexArray();
	vert_array->BindMesh(ys_mesh);
	_scene.vertex_arrays().push_back(vert_array);
}


mat4
ComputeDebugView(const Mesh& _mesh, float _fov)
{
	float H = _mesh.bounds[1].y - _mesh.bounds[0].y;
	float W = _mesh.bounds[1].x - _mesh.bounds[0].x;
	float D = _mesh.bounds[1].z - _mesh.bounds[0].z;

	mat4 view = mat4();
	view.col[3].x = (_mesh.bounds[0].x + W / 2.f);
	view.col[3].y = (_mesh.bounds[0].y + H / 2.f);
	view.col[3].z = (D / 2.f + ((H / 2.f) / (float)tan(_fov* (PI / 180.) / 2.)));

	return view;
}


}

