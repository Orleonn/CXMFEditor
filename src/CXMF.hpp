#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <unordered_map>



#define _CXMF_BEGIN namespace cxmf {
#define _CXMF_END }

#define CXMF_VERSION_MAJOR 1
#define CXMF_VERSION_MINOR 0
#define CXMF_VERSION_PATCH 0
#define CXMF_VERSION_STRING "1.0.0"
#define CXMF_VERSION (::cxmf::make_version(CXMF_VERSION_MAJOR, CXMF_VERSION_MINOR, CXMF_VERSION_PATCH))



_CXMF_BEGIN

constexpr uint32_t make_version(int32_t major, int32_t minor, int32_t patch)
{
	return (static_cast<uint32_t>(major) << 24) |  //
		   (static_cast<uint32_t>(minor) << 16) |  //
		   static_cast<uint32_t>(patch);
}

constexpr void decode_version(uint32_t version, uint32_t& major, uint32_t& minor, uint32_t& patch)
{
	major = (version >> 24) & 0xFF;
	minor = (version >> 16) & 0xFF;
	patch = version & 0xFFFF;
}

constexpr inline uint32_t MAGIC = ((static_cast<uint32_t>('C') << 24) |	 //
								   (static_cast<uint32_t>('X') << 16) |	 //
								   (static_cast<uint32_t>('M') << 8) |	 //
								   static_cast<uint32_t>('F'));

constexpr inline uint32_t NO_PARENT = 0xFFFFFFFFU;
constexpr inline uint32_t NO_BONE = 0xFFFFFFFFU;

enum HEADER_FLAG : uint32_t
{
	HEADER_FLAG_STATIC = 0x00000001,
	HEADER_FLAG_SKINNED = 0x00000002
};

struct HEADER
{
	uint32_t magic;
	uint32_t version;
	uint32_t size;
	uint32_t flags;
};

struct Mat4x4
{
	float _data[16];

	constexpr Mat4x4()
		: _data{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}
	{}

	float& operator[](size_t idx)
	{
		return _data[idx];
	}
	const float& operator[](size_t idx) const
	{
		return _data[idx];
	}
};

struct Vertex
{
	float position[3];
	float normal[3];
	float uv[2];
	float tangent[3];
};

struct WeightedVertex
{
	float position[3];
	float normal[3];
	float uv[2];
	float tangent[3];
	uint32_t boneID[4];
	float weight[4];
};

struct Mesh
{
	std::string name;
	std::string texturesPath;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	float aabbMin[3];
	float aabbMax[3];
};

struct SkinnedMesh
{
	std::string name;
	std::string texturesPath;
	std::vector<WeightedVertex> vertices;
	std::vector<uint32_t> indices;
	float aabbMin[3];
	float aabbMax[3];
};

struct Bone
{
	std::string name;
	Mat4x4 inverseBindTransform;
	uint32_t id;
	uint32_t parentIndex;
};



class StaticModel;
class SkinnedModel;

class IModel
{
protected:
	IModel() = default;

public:
	virtual ~IModel() = default;

public:
	virtual StaticModel* StaticModelCast()
	{
		return nullptr;
	}
	virtual const StaticModel* StaticModelCast() const
	{
		return nullptr;
	}

	virtual SkinnedModel* SkinnedModelCast()
	{
		return nullptr;
	}
	virtual const SkinnedModel* SkinnedModelCast() const
	{
		return nullptr;
	}
};



class StaticModel final : public IModel
{
public:
	std::string name;
	std::vector<Mesh> meshes;
	float aabbMin[3];
	float aabbMax[3];
	uint32_t version;
	uint32_t flags;

public:
	StaticModel();
	~StaticModel() = default;

public:
	StaticModel* StaticModelCast() override
	{
		return this;
	}
	const StaticModel* StaticModelCast() const override
	{
		return this;
	}
};



class SkinnedModel final : public IModel
{
public:
	std::string name;
	std::vector<SkinnedMesh> meshes;
	std::vector<Bone> bones;
	std::vector<Mat4x4> boneOffsets;
	std::unordered_map<std::string, uint32_t> boneMapping;
	uint32_t version;
	uint32_t flags;

public:
	SkinnedModel() = default;
	~SkinnedModel() = default;

public:
	SkinnedModel* SkinnedModelCast() override
	{
		return this;
	}
	const SkinnedModel* SkinnedModelCast() const override
	{
		return this;
	}
};



struct ImportSettings
{
	float angleGSN = 0.0F;
	bool enableGSN = false;
	bool forceStatic = false;
};

extern IModel* ImportModel(const std::string& filePath, const ImportSettings& importInfo, std::string& err);

extern IModel* OpenModel(const std::string& filePath, std::string& err);

extern bool SaveModel(const std::string& filePath, IModel& model, std::string& err);

_CXMF_END
