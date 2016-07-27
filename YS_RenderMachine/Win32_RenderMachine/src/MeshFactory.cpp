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
	Assimp::Importer	main_importer;

	const aiScene*		assimp_scene = main_importer.ReadFile(_file, 0);
	if (!assimp_scene)
		LOG(main_importer.GetErrorString());

	Logger::log_file = "";
	LOG("Loading Nodes..");

	Node*					scene_root = _scene.root();
	std::vector<Mesh*>&		ys_meshes = _scene.meshes();

	// THIS IS GOING TO BE SOME HEAVY PREPROCESSING
	// First we should make the Meshes. 
	// Then, when going through the Assimp scene hierarchy, we will be able to get the mat4*
	// corresponding to our bones.
	
	for (unsigned int i = 0; i < assimp_scene->mNumMeshes; ++i)
	{
		aiMesh*		assimp_mesh = assimp_scene->mMeshes[i];
		Mesh*		ys_mesh = new Mesh();

		for (unsigned int bone_index = 0;
			 bone_index < assimp_mesh->mNumBones; ++bone_index)
		{
			aiBone*		assimp_bone = assimp_mesh->mBones[bone_index];
			Mesh::Bone	ys_bone;
			
			for (unsigned int weight_index = 0;
				 weight_index < assimp_bone->mNumWeights; ++weight_index)
			{
				aiVertexWeight	assimp_weight = 
					assimp_bone->mWeights[weight_index];
				ys_bone.weight_map.emplace(assimp_weight.mVertexId,
										   assimp_weight.mWeight);
			}

			ys_mesh->bones.emplace(assimp_bone->mName.C_Str(), ys_bone);
		}

		for (unsigned int vertex_index = 0;
			 vertex_index < assimp_mesh->mNumVertices; ++vertex_index)
		{
			aiVector3D	assimp_vertex = assimp_mesh->mVertices[vertex_index];
			vec4		ys_vertex(assimp_vertex.x, 
								  assimp_vertex.y, 
								  assimp_vertex.z);

			ys_mesh->vertices.push_back(ys_vertex);
		}

		ys_meshes.push_back(ys_mesh);
	}


	aiNode*					root = assimp_scene->mRootNode;
	std::vector<aiNode*>	assimp_stack;
	std::vector<Node*>		ys_stack;

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

		aiMatrix4x4		intermediate_matrix = assimp_node->mTransformation.Transpose();
		memcpy(ys_node->transform().data,
			   &intermediate_matrix.a1,
			   sizeof(float) * 16);

		for (std::vector<Mesh*>::iterator ite = ys_meshes.begin();
			 ite != ys_meshes.end(); ++ite)
		{
			std::unordered_map<std::string, Mesh::Bone>::iterator bone_ite =
				(*ite)->bones.find(ys_node->name());
			if (bone_ite != (*ite)->bones.end())
			{
				bone_ite->second.transform = &ys_node->transform();
			}
		}

		if (assimp_node->mNumMeshes)
		{
			ys_meshes[assimp_node->mMeshes[0]]->transform =
				&ys_node->transform();
		}

		for (int i = 0; i < assimp_node->mNumChildren; ++i)
		{
			assimp_stack.push_back(assimp_node->mChildren[i]);
			
			Node*	child_node = new Node(assimp_node->mChildren[i]->mName.C_Str());
			ys_node->AddChild(child_node);
			ys_stack.push_back(child_node);
		}
	}

	Logger::log_file = Logger::default_log_file;
}


}

