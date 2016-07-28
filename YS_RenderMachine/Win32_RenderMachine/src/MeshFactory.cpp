#include "MeshFactory.hpp"

#include <iostream>

#include "assimp-3.0/include/assimp/Importer.hpp"
#include "assimp-3.0/include/assimp/scene.h"
#include "assimp-3.0/include/assimp/anim.h"
#include "assimp-3.0/include/assimp/mesh.h"
#include "assimp-3.0/include/assimp/types.h"

#include "Logger.hpp"


namespace ys_render_machine {


void
MeshFactory::LoadIntoScene(Scene& _scene, const std::string& _file)
{
	Logger::Log("Loading Nodes..", Logger::kChannelStdOut);

	Assimp::Importer	main_importer;

	const aiScene*		assimp_scene = main_importer.ReadFile(_file, 0);
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

			Logger::Log(std::to_string(vertex_index) + " " +
						std::to_string(ys_vertex.x) + " " + 
						std::to_string(ys_vertex.y) + " " + 
						std::to_string(ys_vertex.z), 
						Logger::kChannelMeshFactory);
		}

		ys_meshes.push_back(ys_mesh);
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
				bone_ite->second.transform = &ys_node->transform();
			}
		}

		// Every mesh attached to the current node will get a pointer to its
		// transform
		for (unsigned int i = 0; i < assimp_node->mNumMeshes; ++i)
		{
			ys_meshes[assimp_node->mMeshes[i]]->transform =
				&ys_node->transform();
		}

		// Once we have correctly fetched the current node data, we can add its
		// children to our stack so that they can be processed later on.
		for (int i = 0; i < assimp_node->mNumChildren; ++i)
		{
			assimp_stack.push_back(assimp_node->mChildren[i]);
			
			Node*	child_node = new Node(assimp_node->mChildren[i]->mName.C_Str());
			ys_node->AddChild(child_node);
			ys_stack.push_back(child_node);
		}
	}
}


}

