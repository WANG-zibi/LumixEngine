#pragma once
#include "engine/array.h"
#include "engine/hash_map.h"
#include "engine/resource.h"
#include "ffr/ffr.h"
#include "renderer/model.h"


struct lua_State;


namespace Lumix
{


class Renderer;
struct ShaderRenderData;
class Texture;


class LUMIX_RENDERER_API Shader final : public Resource
{
public:
	struct TextureSlot
	{
		TextureSlot()
		{
			name[0] = '\0';
			define_idx = -1;
			default_texture = nullptr;
		}

		char name[32];
		int define_idx = -1;
		Texture* default_texture = nullptr;
	};


	struct Uniform
	{
		enum Type
		{
			INT,
			FLOAT,
			MATRIX4,
			TIME,
			COLOR,
			VEC2,
			VEC3,
			VEC4
		};

		char name[32];
		u32 name_hash;
		Type type;
	// TODO
/*
		bgfx::UniformHandle handle;*/
	};

	struct Source {
		Source(IAllocator& allocator) : code(allocator) {}
		ffr::ShaderType type;
		Array<char> code;
	};

	struct Program {
		int attribute_by_semantics[16];
		bool use_semantics;
		ffr::ProgramHandle handle;
	};

	struct AttributeInfo {
		StaticString<32> name;
		Mesh::AttributeSemantic semantic;
	};

public:
	static const int MAX_TEXTURE_SLOT_COUNT = 16;
	ShaderRenderData* m_render_data;

public:
	Shader(const Path& path, ResourceManager& resource_manager, Renderer& renderer, IAllocator& allocator);
	~Shader();

	ResourceType getType() const override { return TYPE; }

	static const Program& getProgram(ShaderRenderData* rd, u32 defines);

	IAllocator& m_allocator;
	Renderer& m_renderer;
	u32 m_all_defines_mask;
	u64 m_render_states;
	TextureSlot m_texture_slots[MAX_TEXTURE_SLOT_COUNT];
	int m_texture_slot_count;
	Array<Uniform> m_uniforms;

	static const ResourceType TYPE;

private:
	bool generateInstances();

	void unload() override;
	bool load(u64 size, const u8* mem) override;
};


struct ShaderRenderData 
{
	ShaderRenderData(Renderer& renderer, IAllocator& allocator) 
		: allocator(allocator)
		, renderer(renderer)
		, programs(allocator) 
		, include(allocator)
		, sources(allocator)
		, common_source(allocator)
		, attributes(allocator)
	{}
	IAllocator& allocator;
	Renderer& renderer;
	HashMap<u32, Shader::Program> programs;
	Array<Shader::Source> sources;
	Array<char> include;
	Array<char> common_source;
	Array<Shader::AttributeInfo> attributes;
	Path path;
};

} // namespace Lumix
