
#include "CXMF.hpp"

#include <list>
#include <limits>
#include <fstream>
#include <memory>
#include <filesystem>
#include <algorithm>
#include <format>
#include <cassert>



// cxmf::Mesh

static inline std::ostream& operator<<(std::ostream& _Out, const cxmf::Mesh& mesh)
{
	const uint32_t nameLen = static_cast<uint32_t>(mesh.name.length());
	const uint32_t texPathLen = static_cast<uint32_t>(mesh.texturesPath.length());
	const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
	const uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());

	_Out.write(reinterpret_cast<const char*>(&nameLen), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&texPathLen), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&vertexCount), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&indexCount), sizeof(uint32_t));
	_Out.write(mesh.name.c_str(), mesh.name.length());
	_Out.write(mesh.texturesPath.c_str(), mesh.texturesPath.length());
	_Out.write(reinterpret_cast<const char*>(mesh.vertices.data()), sizeof(cxmf::Vertex) * vertexCount);
	_Out.write(reinterpret_cast<const char*>(mesh.indices.data()), sizeof(uint32_t) * indexCount);
	_Out.write(reinterpret_cast<const char*>(&mesh.aabbMin[0]), sizeof(mesh.aabbMin));
	_Out.write(reinterpret_cast<const char*>(&mesh.aabbMax[0]), sizeof(mesh.aabbMax));
	return _Out;
}

static inline std::istream& operator>>(std::istream& _In, cxmf::Mesh& mesh)
{
	uint32_t nameLen = 0;
	uint32_t texPathLen = 0;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;

	_In.read(reinterpret_cast<char*>(&nameLen), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&texPathLen), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&vertexCount), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&indexCount), sizeof(uint32_t));

	mesh.name.resize(nameLen, '\0');
	mesh.texturesPath.resize(texPathLen, '\0');
	mesh.vertices.resize(vertexCount);
	mesh.indices.resize(indexCount);

	_In.read(mesh.name.data(), nameLen);
	_In.read(mesh.texturesPath.data(), texPathLen);
	_In.read(reinterpret_cast<char*>(mesh.vertices.data()), sizeof(cxmf::Vertex) * vertexCount);
	_In.read(reinterpret_cast<char*>(mesh.indices.data()), sizeof(uint32_t) * indexCount);
	_In.read(reinterpret_cast<char*>(&mesh.aabbMin[0]), sizeof(mesh.aabbMin));
	_In.read(reinterpret_cast<char*>(&mesh.aabbMax[0]), sizeof(mesh.aabbMax));
	return _In;
}

// cxmf::SkinnedMesh

static inline std::ostream& operator<<(std::ostream& _Out, const cxmf::SkinnedMesh& mesh)
{
	const uint32_t nameLen = static_cast<uint32_t>(mesh.name.length());
	const uint32_t texPathLen = static_cast<uint32_t>(mesh.texturesPath.length());
	const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
	const uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());

	_Out.write(reinterpret_cast<const char*>(&nameLen), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&texPathLen), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&vertexCount), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&indexCount), sizeof(uint32_t));

	_Out.write(mesh.name.c_str(), mesh.name.length());
	_Out.write(mesh.texturesPath.c_str(), mesh.texturesPath.length());
	_Out.write(reinterpret_cast<const char*>(mesh.vertices.data()), sizeof(cxmf::WeightedVertex) * vertexCount);
	_Out.write(reinterpret_cast<const char*>(mesh.indices.data()), sizeof(uint32_t) * indexCount);
	_Out.write(reinterpret_cast<const char*>(&mesh.aabbMin[0]), sizeof(mesh.aabbMin));
	_Out.write(reinterpret_cast<const char*>(&mesh.aabbMax[0]), sizeof(mesh.aabbMax));
	return _Out;
}

static inline std::istream& operator>>(std::istream& _In, cxmf::SkinnedMesh& mesh)
{
	uint32_t nameLen = 0;
	uint32_t texPathLen = 0;
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;

	_In.read(reinterpret_cast<char*>(&nameLen), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&texPathLen), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&vertexCount), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&indexCount), sizeof(uint32_t));

	mesh.name.resize(nameLen, '\0');
	mesh.texturesPath.resize(texPathLen, '\0');
	mesh.vertices.resize(vertexCount);
	mesh.indices.resize(indexCount);

	_In.read(mesh.name.data(), nameLen);
	_In.read(mesh.texturesPath.data(), texPathLen);
	_In.read(reinterpret_cast<char*>(mesh.vertices.data()), sizeof(cxmf::WeightedVertex) * vertexCount);
	_In.read(reinterpret_cast<char*>(mesh.indices.data()), sizeof(uint32_t) * indexCount);
	_In.read(reinterpret_cast<char*>(&mesh.aabbMin[0]), sizeof(mesh.aabbMin));
	_In.read(reinterpret_cast<char*>(&mesh.aabbMax[0]), sizeof(mesh.aabbMax));
	return _In;
}

// cxmf::Bone

static inline std::ostream& operator<<(std::ostream& _Out, const cxmf::Bone& bone)
{
	const uint32_t nameLen = static_cast<uint32_t>(bone.name.length());

	_Out.write(reinterpret_cast<const char*>(&nameLen), sizeof(uint32_t));
	_Out.write(bone.name.c_str(), bone.name.length());
	_Out.write(reinterpret_cast<const char*>(&bone.inverseBindTransform[0]), sizeof(cxmf::Mat4x4));
	_Out.write(reinterpret_cast<const char*>(&bone.parentIndex), sizeof(uint32_t));
	return _Out;
}

static inline std::istream& operator>>(std::istream& _In, cxmf::Bone& bone)
{
	uint32_t nameLen = 0;

	_In.read(reinterpret_cast<char*>(&nameLen), sizeof(uint32_t));

	bone.name.resize(nameLen, '\0');

	_In.read(bone.name.data(), nameLen);
	_In.read(reinterpret_cast<char*>(&bone.inverseBindTransform[0]), sizeof(cxmf::Mat4x4));
	_In.read(reinterpret_cast<char*>(&bone.parentIndex), sizeof(uint32_t));
	return _In;
}

// cxmf::StaticModel

static inline std::ostream& operator<<(std::ostream& _Out, const cxmf::StaticModel& model)
{
	const uint32_t nameLen = static_cast<uint32_t>(model.name.length());
	const uint32_t meshesCount = static_cast<uint32_t>(model.meshes.size());

	_Out.write(reinterpret_cast<const char*>(&nameLen), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&meshesCount), sizeof(uint32_t));
	_Out.write(model.name.c_str(), model.name.length());
	_Out.write(reinterpret_cast<const char*>(&model.aabbMin[0]), sizeof(model.aabbMin));
	_Out.write(reinterpret_cast<const char*>(&model.aabbMax[0]), sizeof(model.aabbMax));

	for (uint32_t i = 0; i < meshesCount; ++i)
	{
		_Out << model.meshes[i];
	}
	return _Out;
}

static inline std::istream& operator>>(std::istream& _In, cxmf::StaticModel& model)
{
	uint32_t nameLen = 0;
	uint32_t meshesCount = 0;

	_In.read(reinterpret_cast<char*>(&nameLen), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&meshesCount), sizeof(uint32_t));

	model.name.resize(nameLen, '\0');
	model.meshes.resize(meshesCount);

	_In.read(model.name.data(), nameLen);
	_In.read(reinterpret_cast<char*>(&model.aabbMin[0]), sizeof(model.aabbMin));
	_In.read(reinterpret_cast<char*>(&model.aabbMax[0]), sizeof(model.aabbMax));

	for (uint32_t i = 0; i < meshesCount; ++i)
	{
		_In >> model.meshes[i];
	}
	return _In;
}

// cxmf::SkinnedModel

static inline std::ostream& operator<<(std::ostream& _Out, const cxmf::SkinnedModel& model)
{
	const uint32_t nameLen = static_cast<uint32_t>(model.name.length());
	const uint32_t meshesCount = static_cast<uint32_t>(model.meshes.size());
	const uint32_t bonesCount = static_cast<uint32_t>(model.bones.size());

	_Out.write(reinterpret_cast<const char*>(&nameLen), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&meshesCount), sizeof(uint32_t));
	_Out.write(reinterpret_cast<const char*>(&bonesCount), sizeof(uint32_t));
	_Out.write(model.name.c_str(), model.name.length());

	for (uint32_t i = 0; i < meshesCount; ++i)
	{
		_Out << model.meshes[i];
	}
	for (uint32_t i = 0; i < bonesCount; ++i)
	{
		_Out << model.bones[i];
	}

	_Out.write(reinterpret_cast<const char*>(model.boneOffsets.data()), bonesCount * sizeof(cxmf::Mat4x4));
	return _Out;
}

static inline std::istream& operator>>(std::istream& _In, cxmf::SkinnedModel& model)
{
	uint32_t nameLen = 0;
	uint32_t meshesCount = 0;
	uint32_t bonesCount = 0;

	_In.read(reinterpret_cast<char*>(&nameLen), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&meshesCount), sizeof(uint32_t));
	_In.read(reinterpret_cast<char*>(&bonesCount), sizeof(uint32_t));

	model.name.resize(nameLen, '\0');
	model.meshes.resize(meshesCount);
	model.bones.resize(bonesCount);
	model.boneOffsets.resize(bonesCount);
	model.boneMapping.reserve(bonesCount);

	_In.read(model.name.data(), nameLen);

	for (uint32_t i = 0; i < meshesCount; ++i)
	{
		_In >> model.meshes[i];
	}
	for (uint32_t i = 0; i < bonesCount; ++i)
	{
		cxmf::Bone& bone = model.bones[i];

		_In >> bone;

		bone.id = i;
		model.boneMapping[bone.name] = i;
	}

	_In.read(reinterpret_cast<char*>(model.boneOffsets.data()), bonesCount * sizeof(cxmf::Mat4x4));
	return _In;
}





#include "AssimpDefs.hpp"



_CXMF_BEGIN

StaticModel::StaticModel()
	: name(), meshes(), aabbMin{}, aabbMax{}, version(0), flags(0)
{
	constexpr float minVal = std::numeric_limits<float>::max();
	constexpr float maxVal = -std::numeric_limits<float>::max();
	for (int i = 0; i < 3; ++i)
	{
		aabbMin[i] = minVal;
		aabbMax[i] = maxVal;
	}
}





struct ImportModelInfo
{
	std::string modelName;
	uint32_t flags = 0;
};



static std::string get_base_texture_path(const aiMesh& mesh, const aiScene& scene)
{
	std::string outPath;
	if (mesh.mMaterialIndex >= scene.mNumMaterials)	 //
		return outPath;

	aiString path;
	const aiMaterial& material = *scene.mMaterials[mesh.mMaterialIndex];
	const aiReturn result = material.GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &path);
	if (result == aiReturn_SUCCESS)
	{
		outPath = path.C_Str();
	}
	return outPath;
}

static void assimp_matrix_to_cxmf(Mat4x4& out, const aiMatrix4x4& in)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			out[col * 4 + row] = in[row][col];
		}
	}
}



static StaticModel* import_static_model(const aiScene& scene, ImportModelInfo& importInfo)
{
	StaticModel* const model = new StaticModel();

	model->version = CXMF_VERSION;
	model->flags = (HEADER_FLAG_STATIC | importInfo.flags);
	model->name = importInfo.modelName;
	model->meshes.resize(scene.mNumMeshes);
	for (uint32_t i_mesh = 0; i_mesh < scene.mNumMeshes; ++i_mesh)
	{
		const aiMesh& sceneMesh = *scene.mMeshes[i_mesh];
		const bool hasTextureCoords = sceneMesh.HasTextureCoords(0);

		Mesh& mesh = model->meshes[i_mesh];
		mesh.name = sceneMesh.mName.C_Str();
		mesh.texturesPath = get_base_texture_path(sceneMesh, scene);

		for (int i = 0; i < 3; ++i)
		{
			mesh.aabbMin[i] = sceneMesh.mAABB.mMin[i];
			mesh.aabbMax[i] = sceneMesh.mAABB.mMax[i];

			model->aabbMin[i] = std::min<float>(model->aabbMin[i], mesh.aabbMin[i]);
			model->aabbMax[i] = std::max<float>(model->aabbMax[i], mesh.aabbMax[i]);
		}

		// Verticies
		mesh.vertices.resize(sceneMesh.mNumVertices);
		for (uint32_t i_vertex = 0; i_vertex < sceneMesh.mNumVertices; ++i_vertex)
		{
			Vertex& vert = mesh.vertices[i_vertex];
			const aiVector3D& position = sceneMesh.mVertices[i_vertex];
			const aiVector3D& normal = sceneMesh.mNormals[i_vertex];
			const aiVector3D& tangent = sceneMesh.mTangents[i_vertex];
			for (int i = 0; i < 3; ++i)
			{
				vert.position[i] = position[i];
				vert.normal[i] = normal[i];
				vert.tangent[i] = tangent[i];
			}

			if (hasTextureCoords)
			{
				const aiVector3D& texCoords = sceneMesh.mTextureCoords[0][i_vertex];
				vert.uv[0] = texCoords[0];
				vert.uv[1] = texCoords[1];
			}
			else
			{
				vert.uv[0] = 0.0F;
				vert.uv[1] = 0.0F;
			}
		}

		// Indices
		mesh.indices.resize(static_cast<size_t>(sceneMesh.mNumFaces) * 3);
		for (uint32_t i_face = 0; i_face < sceneMesh.mNumFaces; ++i_face)
		{
			const aiFace& face = sceneMesh.mFaces[i_face];
			const uint32_t offset = i_face * 3;
			mesh.indices[offset + 0] = face.mIndices[0];
			mesh.indices[offset + 1] = face.mIndices[1];
			mesh.indices[offset + 2] = face.mIndices[2];
		}
	}
	return model;
}



static uint32_t add_bone(SkinnedModel& model, std::list<Bone>& bones, const aiBone& bone, const aiScene& scene)
{
	const std::string boneName = bone.mName.C_Str();
	{
		const auto _It = model.boneMapping.find(boneName);
		if (_It != model.boneMapping.end())	 //
			return _It->second;
	}

	const aiNode& boneNode = *bone.mNode;
	const uint32_t boneIndex = static_cast<uint32_t>(bones.size());
	model.boneMapping[boneName] = boneIndex;
	assimp_matrix_to_cxmf(model.boneOffsets.emplace_back(), bone.mOffsetMatrix);

	Bone& newBone = bones.emplace_back();
	newBone.name = boneName;
	assimp_matrix_to_cxmf(newBone.inverseBindTransform, boneNode.mTransformation);
	newBone.id = boneIndex;
	if (boneNode.mParent == nullptr)
	{
		newBone.parentIndex = NO_PARENT;
	}
	else
	{
		const aiBone* const parentBone = scene.findBone(boneNode.mParent->mName);
		if (parentBone == nullptr)
		{
			newBone.parentIndex = NO_PARENT;
		}
		else
		{
			newBone.parentIndex = add_bone(model, bones, *parentBone, scene);
			if (newBone.parentIndex == newBone.id)
			{
				newBone.parentIndex = NO_PARENT;
			}
		}
	}
	return boneIndex;
}

static SkinnedModel* import_skinned_model(const aiScene& scene, ImportModelInfo& importInfo)
{
	SkinnedModel* const model = new SkinnedModel();
	std::list<Bone> tempBones;

	model->name = importInfo.modelName;
	model->version = CXMF_VERSION;
	model->flags = (HEADER_FLAG_SKINNED | importInfo.flags);

	model->meshes.resize(scene.mNumMeshes);
	std::string tmpBoneName;
	std::vector<float> goodWeights;
	goodWeights.reserve(4);
	for (uint32_t i_mesh = 0; i_mesh < scene.mNumMeshes; ++i_mesh)
	{
		const aiMesh& sceneMesh = *scene.mMeshes[i_mesh];
		const bool hasTextureCoords = sceneMesh.HasTextureCoords(0);

		SkinnedMesh& mesh = model->meshes[i_mesh];
		mesh.name = sceneMesh.mName.C_Str();
		mesh.texturesPath = get_base_texture_path(sceneMesh, scene);

		for (int i = 0; i < 3; ++i)
		{
			mesh.aabbMin[i] = sceneMesh.mAABB.mMin[i];
			mesh.aabbMax[i] = sceneMesh.mAABB.mMax[i];
		}

		// Bones
		for (uint32_t i_bone = 0; i_bone < sceneMesh.mNumBones; ++i_bone)
		{
			add_bone(*model, tempBones, *sceneMesh.mBones[i_bone], scene);
		}

		// Verticies
		mesh.vertices.resize(sceneMesh.mNumVertices);
		for (uint32_t i_vertex = 0; i_vertex < sceneMesh.mNumVertices; ++i_vertex)
		{
			WeightedVertex& vert = mesh.vertices[i_vertex];
			const aiVector3D& position = sceneMesh.mVertices[i_vertex];
			const aiVector3D& normal = sceneMesh.mNormals[i_vertex];
			const aiVector3D& tangent = sceneMesh.mTangents[i_vertex];
			for (int i = 0; i < 3; ++i)
			{
				vert.position[i] = position[i];
				vert.normal[i] = normal[i];
				vert.tangent[i] = tangent[i];
			}

			for (int i = 0; i < 4; ++i)
			{
				vert.boneID[i] = NO_BONE;
				vert.weight[i] = 0.0F;
			}

			if (hasTextureCoords)
			{
				const aiVector3D& texCoords = sceneMesh.mTextureCoords[0][i_vertex];
				vert.uv[0] = texCoords[0];
				vert.uv[1] = texCoords[1];
			}
			else
			{
				vert.uv[0] = 0.0F;
				vert.uv[1] = 0.0F;
			}
		}

		// Indices
		mesh.indices.resize(static_cast<size_t>(sceneMesh.mNumFaces) * 3);
		for (uint32_t i_face = 0; i_face < sceneMesh.mNumFaces; ++i_face)
		{
			const aiFace& face = sceneMesh.mFaces[i_face];
			const uint32_t offset = i_face * 3;
			mesh.indices[offset + 0] = face.mIndices[0];
			mesh.indices[offset + 1] = face.mIndices[1];
			mesh.indices[offset + 2] = face.mIndices[2];
		}

		// Weights and bone IDs
		if (sceneMesh.mNumBones > 0)
		{
			std::vector<uint32_t> influenceOnVerticies(mesh.vertices.size(), 0);
			for (uint32_t i_bone = 0; i_bone < sceneMesh.mNumBones; ++i_bone)
			{
				const aiBone& meshBone = *sceneMesh.mBones[i_bone];
				tmpBoneName = meshBone.mName.C_Str();
				const uint32_t boneIndex = model->boneMapping[tmpBoneName];

				for (uint32_t i_weight = 0; i_weight < meshBone.mNumWeights; ++i_weight)
				{
					const aiVertexWeight& vertWeight = meshBone.mWeights[i_weight];
					WeightedVertex& vertex = mesh.vertices[vertWeight.mVertexId];
					uint32_t& coord = influenceOnVerticies[vertWeight.mVertexId];
					if (coord < 4)
					{
						vertex.boneID[coord] = boneIndex;
						vertex.weight[coord] = vertWeight.mWeight;
						++coord;
					}
				}
			}
		}

		// Normalize weights
		for (uint32_t i_vertex = 0; i_vertex < sceneMesh.mNumVertices; ++i_vertex)
		{
			goodWeights.clear();
			WeightedVertex& vert = mesh.vertices[i_vertex];
			for (int i = 0; i < 4; ++i)
			{
				float& weight = vert.weight[i];
				if (weight < 0.05F)
					weight = 0.0F;
				else
					goodWeights.push_back(weight);
			}

			if (!goodWeights.empty())
			{
				float mag = 0.0F;
				for (float w : goodWeights)
					mag += w;

				uint32_t goodID = 0;
				for (int i = 0; i < 4; ++i)
				{
					float& weight = vert.weight[i];
					if (weight > 0.0F)	//
						weight = goodWeights[goodID++] / mag;
				}
			}
		}
	}

	model->bones.reserve(tempBones.size());
	for (Bone& b : tempBones)
		model->bones.push_back(std::move(b));

	return model;
}

IModel* ImportModel(const std::string& filePath, const ImportSettings& importInfo, std::string& err)
{
	err.clear();
	cxmfScopeLogger logger(err, Assimp::Logger::LogSeverity::NORMAL,  //
						   Assimp::Logger::ErrorSeverity::Warn | Assimp::Logger::ErrorSeverity::Err);

	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_PP_FD_REMOVE, true);
	importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 4);
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

	uint32_t importFlags = aiProcess_CalcTangentSpace |			 //
						   aiProcess_JoinIdenticalVertices |	 //
						   aiProcess_Triangulate |				 //
						   aiProcess_LimitBoneWeights |			 //
						   aiProcess_ValidateDataStructure |	 //
						   aiProcess_ImproveCacheLocality |		 //
						   aiProcess_RemoveRedundantMaterials |	 //
						   aiProcess_PopulateArmatureData |		 //
						   aiProcess_SortByPType |				 //
						   // aiProcess_OptimizeMeshes |			  //
						   // aiProcess_OptimizeGraph |			  //
						   aiProcess_FlipUVs |	//
						   aiProcess_GenBoundingBoxes;

	if (importInfo.enableGSN)
	{
		importFlags |= aiProcess_GenSmoothNormals;
		const float angle = std::clamp<float>(importInfo.angleGSN, 5.0F, 175.0F);
		importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, angle);
	}

	const aiScene* const scene = importer.ReadFile(filePath, importFlags);
	if (!scene) return nullptr;

	if (!scene->HasMeshes())
	{
		ASSIMP_LOG_ERROR("Model has no meshes!");
		return nullptr;
	}

	bool has_Bones = false;
	for (uint32_t i = 0; i < scene->mNumMeshes; ++i)
	{
		if (scene->mMeshes[i]->HasBones())
		{
			has_Bones = true;
			break;
		}
	}

	ImportModelInfo info;
	info.modelName = std::filesystem::path(filePath).stem().string();
	info.flags = 0;

	if (!importInfo.forceStatic && has_Bones)
		return import_skinned_model(*scene, info);
	else
		return import_static_model(*scene, info);
}

IModel* OpenModel(const std::string& filePath, std::string& err)
{
	err.clear();

	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		err += "Cannot open file!\n";
		return nullptr;
	}

	file.seekg(0, std::ios::end);

	const size_t fileSize = static_cast<size_t>(file.tellg());
	if (fileSize <= sizeof(HEADER))
	{
		err += "Invalid file size!\n";
		return nullptr;
	}

	file.seekg(0, std::ios::beg);

	HEADER header{};
	file.read(reinterpret_cast<char*>(&header), sizeof(HEADER));

	if (header.magic != MAGIC)
	{
		err += "Invalid file format!\n";
		return nullptr;
	}

	if (static_cast<size_t>(header.size) != fileSize)
	{
		err += "Model is damaged!\n";
		return nullptr;
	}

	uint32_t ver_major, ver_minor, ver_patch;
	decode_version(header.version, ver_major, ver_minor, ver_patch);
	if (ver_major != CXMF_VERSION_MAJOR || ver_minor != CXMF_VERSION_MINOR)
	{
		err += std::format("Invalid model version: {}.{}.{}\n", ver_major, ver_minor, ver_patch);
		return nullptr;
	}

	IModel* outModel;
	if (header.flags & HEADER_FLAG_STATIC)
	{
		StaticModel* const model = new StaticModel();
		model->version = header.version;
		model->flags = header.flags;
		file >> *model;
		outModel = model;
	}
	else if (header.flags & HEADER_FLAG_SKINNED)
	{
		SkinnedModel* const model = new SkinnedModel();
		model->version = header.version;
		model->flags = header.flags;
		file >> *model;
		outModel = model;
	}
	else
	{
		outModel = nullptr;
		err += "Invalid model type!\n";
	}
	return outModel;
}

template <typename _ModelTy>
requires (std::is_same_v<_ModelTy, StaticModel> || std::is_same_v<_ModelTy, SkinnedModel>)
static bool save_model_impl(std::ofstream& file, const std::string& filePath, _ModelTy& model)
{
	if (model.name.empty())
	{
		model.name = std::filesystem::path(filePath).stem().string();
	}

	HEADER header{};
	header.magic = MAGIC;
	header.version = CXMF_VERSION;
	header.size = 0;
	header.flags = model.flags;
	file.write(reinterpret_cast<const char*>(&header), sizeof(HEADER));
	file << model;
	const uint32_t fileSize = static_cast<uint32_t>(file.tellp());
	file.seekp(offsetof(HEADER, size), std::ios::beg);
	file.write(reinterpret_cast<const char*>(&fileSize), sizeof(uint32_t));
	return true;
}

bool SaveModel(const std::string& filePath, IModel& model, std::string& err)
{
	assert(model.StaticModelCast() != nullptr || model.SkinnedModelCast() != nullptr);

	err.clear();

	std::ofstream file(filePath, std::ios::binary | std::ios::trunc);
	if (!file.is_open())
	{
		err += "Cannot open file!\n";
		return false;
	}

	if (model.SkinnedModelCast() != nullptr)
	{
		return save_model_impl(file, filePath, *model.SkinnedModelCast());
	}
	else
	{
		return save_model_impl(file, filePath, *model.StaticModelCast());
	}
}

_CXMF_END
