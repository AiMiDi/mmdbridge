﻿#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include "EncodingHelper.h"

namespace pmd
{
	/// ヘッダ
	class PmdHeader
	{
	public:
		/// モデル名
		std::string name;
		/// モデル名(英語)
		std::string name_english;
		/// コメント
		std::string comment;
		/// コメント(英語)
		std::string comment_english;

		bool Read(std::ifstream* stream)
		{
			char buffer[256];
			stream->read(buffer, 20);
			name = std::string(buffer);
			stream->read(buffer, 256);
			comment = std::string(buffer);
			return true;
		}

		bool ReadExtension(std::ifstream* stream)
		{
			char buffer[256];
			stream->read(buffer, 20);
			name_english = std::string(buffer);
			stream->read(buffer, 256);
			comment_english = std::string(buffer);
			return true;
		}
	};

	/// 頂点
	class PmdVertex
	{
	public:
		/// 位置
		float position[3];

		/// 法線
		float normal[3];

		/// UV座標
		float uv[2];

		/// 関連ボーンインデックス
		uint16_t bone_index[2];

		/// ボーンウェイト
		uint8_t bone_weight;

		/// エッジ不可視
		bool edge_invisible;

		bool Read(std::ifstream* stream)
		{
			stream->read((char*) position, sizeof(float) * 3);
			stream->read((char*) normal, sizeof(float) * 3);
			stream->read((char*) uv, sizeof(float) * 2);
			stream->read((char*) bone_index, sizeof(uint16_t) * 2);
			stream->read((char*) &bone_weight, sizeof(uint8_t));
			stream->read((char*) &edge_invisible, sizeof(uint8_t));
			return true;
		}
	};

	/// 材質
	class PmdMaterial
	{
	public:
		/// 減衰色
		float diffuse[4];
		/// 光沢度
		float power;
		/// 光沢色
		float specular[3];
		/// 環境色
		float ambient[3];
		/// トーンインデックス
		uint8_t toon_index;
		/// エッジ
		uint8_t edge_flag;
		/// インデックス数
		uint32_t index_count;
		/// テクスチャファイル名
		std::string texture_filename;
		/// スフィアファイル名
		std::string sphere_filename;

		bool Read(std::ifstream* stream)
		{
			char buffer[20];
			stream->read((char*) &diffuse, sizeof(float) * 4);
			stream->read((char*) &power, sizeof(float));
			stream->read((char*) &specular, sizeof(float) * 3);
			stream->read((char*) &ambient, sizeof(float) * 3);
			stream->read((char*) &toon_index, sizeof(uint8_t));
			stream->read((char*) &edge_flag, sizeof(uint8_t));
			stream->read((char*) &index_count, sizeof(uint32_t));
			stream->read((char*) &buffer, sizeof(char) * 20);
			char* pstar = strchr(buffer, '*');
			if (NULL == pstar)
			{
				texture_filename = std::string(buffer);
				sphere_filename.clear();
			}
			else {
				*pstar = NULL;
				texture_filename = std::string(buffer);
				sphere_filename = std::string(pstar+1);
			}
			return true;
		}
	};

	enum class BoneType : uint8_t
	{
		Rotation,
		RotationAndMove,
		IkEffector,
		Unknown,
		IkEffectable,
		RotationEffectable,
		IkTarget,
		Invisible,
		Twist,
		RotationMovement
	};

	/// ボーン
	class PmdBone
	{
	public:
		/// ボーン名
		std::string name;
		/// ボーン名(英語)
		std::string name_english;
		/// 親ボーン番号
		uint16_t parent_bone_index;
		/// 末端ボーン番号
		uint16_t tail_pos_bone_index;
		/// ボーン種類
		BoneType bone_type;
		/// IKボーン番号
		uint16_t ik_parent_bone_index;
		/// ボーンのヘッドの位置
		float bone_head_pos[3];

		void Read(std::istream *stream)
		{
			char buffer[20];
			stream->read(buffer, 20);
			name = std::string(buffer);
			stream->read((char*) &parent_bone_index, sizeof(uint16_t));
			stream->read((char*) &tail_pos_bone_index, sizeof(uint16_t));
			stream->read((char*) &bone_type, sizeof(uint8_t));
			stream->read((char*) &ik_parent_bone_index, sizeof(uint16_t));
			stream->read((char*) &bone_head_pos, sizeof(float) * 3);
		}

		void ReadExpantion(std::istream *stream)
		{
			char buffer[20];
			stream->read(buffer, 20);
			name_english = std::string(buffer);
		}
	};

	/// IK
	class PmdIk
	{
	public:
		/// IKボーン番号
		uint16_t ik_bone_index;
		/// IKターゲットボーン番号
		uint16_t target_bone_index;
		/// 再帰回数
		uint16_t interations;
		/// 角度制限
		float angle_limit;
		/// 影響下ボーン番号
		std::vector<uint16_t> ik_child_bone_index;

		void Read(std::istream *stream)
		{
			stream->read((char *) &ik_bone_index, sizeof(uint16_t));
			stream->read((char *) &target_bone_index, sizeof(uint16_t));
			uint8_t ik_chain_length;
			stream->read((char*) &ik_chain_length, sizeof(uint8_t));
			stream->read((char *) &interations, sizeof(uint16_t));
			stream->read((char *) &angle_limit, sizeof(float));
			ik_child_bone_index.resize(ik_chain_length);
			for (int i = 0; i < ik_chain_length; i++)
			{
				stream->read((char *) &ik_child_bone_index[i], sizeof(uint16_t));
			}
		}
	};

	class PmdFaceVertex
	{
	public:
		int vertex_index;
		float position[3];

		void Read(std::istream *stream)
		{
			stream->read((char *) &vertex_index, sizeof(int));
			stream->read((char *) position, sizeof(float) * 3);
		}
	};

	enum class FaceCategory : uint8_t
	{
		Base,
		Eyebrow,
		Eye,
		Mouth,
		Other
	};

	class PmdFace
	{
	public:
		std::string name;
		FaceCategory type;
		std::vector<PmdFaceVertex> vertices;
		std::string name_english;

		void Read(std::istream *stream)
		{
			char buffer[20];
			stream->read(buffer, 20);
			name = std::string(buffer);
			int vertex_count;
			stream->read((char*) &vertex_count, sizeof(int));
			stream->read((char*) &type, sizeof(uint8_t));
			vertices.resize(vertex_count);
			for (int i = 0; i < vertex_count; i++)
			{
				vertices[i].Read(stream);
			}
		}

		void ReadExpantion(std::istream *stream)
		{
			char buffer[20];
			stream->read(buffer, 20);
			name_english = std::string(buffer);
		}
	};

	/// ボーン枠用の枠名
	class PmdBoneDispName
	{
	public:
		std::string bone_disp_name;
		std::string bone_disp_name_english;

		void Read(std::istream *stream)
		{
			char buffer[50];
			stream->read(buffer, 50);
			bone_disp_name = std::string(buffer);
			bone_disp_name_english.clear();
		}
		void ReadExpantion(std::istream *stream)
		{
			char buffer[50];
			stream->read(buffer, 50);
			bone_disp_name_english = std::string(buffer);
		}
	};

	class PmdBoneDisp
	{
	public:
		uint16_t bone_index;
		uint8_t bone_disp_index;

		void Read(std::istream *stream)
		{
			stream->read((char*) &bone_index, sizeof(uint16_t));
			stream->read((char*) &bone_disp_index, sizeof(uint8_t));
		}
	};

	/// 衝突形状
	enum class RigidBodyShape : uint8_t
	{
		/// 球
		Sphere = 0,
		/// 直方体
		Box = 1,
		/// カプセル
		Cpusel = 2
	};

	/// 剛体タイプ
	enum class RigidBodyType : uint8_t
	{
		/// ボーン追従
		BoneConnected = 0,
		/// 物理演算
		Physics = 1,
		/// 物理演算(Bone位置合せ)
		ConnectedPhysics = 2
	};

	/// 剛体
	class PmdRigidBody
	{
	public:
		/// 名前
		std::string name;
		/// 関連ボーン番号
		uint16_t related_bone_index;
		/// グループ番号
		uint8_t group_index;
		/// マスク
		uint16_t mask;
		/// 形状
		RigidBodyShape shape;
		/// 大きさ
		float size[3];
		/// 位置
		float position[3];
		/// 回転
		float orientation[3];
		/// 質量
		float weight;
		/// 移動ダンピング
		float linear_damping;
		/// 回転ダンピング
		float anglar_damping;
		/// 反発係数
		float restitution;
		/// 摩擦係数
		float friction;
		/// 演算方法
		RigidBodyType rigid_type;

		void Read(std::istream *stream)
		{
			char buffer[20];
			stream->read(buffer, sizeof(char) * 20);
			name = (std::string(buffer));
			stream->read((char*) &related_bone_index, sizeof(uint16_t));
			stream->read((char*) &group_index, sizeof(uint8_t));
			stream->read((char*) &mask, sizeof(uint16_t));
			stream->read((char*) &shape, sizeof(uint8_t));
			stream->read((char*) size, sizeof(float) * 3);
			stream->read((char*) position, sizeof(float) * 3);
			stream->read((char*) orientation, sizeof(float) * 3);
			stream->read((char*) &weight, sizeof(float));
			stream->read((char*) &linear_damping, sizeof(float));
			stream->read((char*) &anglar_damping, sizeof(float));
			stream->read((char*) &restitution, sizeof(float));
			stream->read((char*) &friction, sizeof(float));
			stream->read((char*) &rigid_type, sizeof(char));
		}
	};

	/// 剛体の拘束
	class PmdConstraint
	{
	public:
		/// 名前
		std::string name;
		/// 剛体Aのインデックス
		uint32_t rigid_body_index_a;
		/// 剛体Bのインデックス
		uint32_t rigid_body_index_b;
		/// 位置
		float position[3];
		/// 回転
		float orientation[3];
		/// 最小移動制限
		float linear_lower_limit[3];
		/// 最大移動制限
		float linear_upper_limit[3];
		/// 最小回転制限
		float angular_lower_limit[3];
		/// 最大回転制限
		float angular_upper_limit[3];
		/// 移動に対する復元力
		float linear_stiffness[3];
		/// 回転に対する復元力
		float angular_stiffness[3];

		void Read(std::istream *stream)
		{
			char buffer[20];
			stream->read(buffer, 20);
			name = std::string(buffer);
			stream->read((char *) &rigid_body_index_a, sizeof(uint32_t));
			stream->read((char *) &rigid_body_index_b, sizeof(uint32_t));
			stream->read((char *) position, sizeof(float) * 3);
			stream->read((char *) orientation, sizeof(float) * 3);
			stream->read((char *) linear_lower_limit, sizeof(float) * 3);
			stream->read((char *) linear_upper_limit, sizeof(float) * 3);
			stream->read((char *) angular_lower_limit, sizeof(float) * 3);
			stream->read((char *) angular_upper_limit, sizeof(float) * 3);
			stream->read((char *) linear_stiffness, sizeof(float) * 3);
			stream->read((char *) angular_stiffness, sizeof(float) * 3);
		}
	};

	/// PMDモデル
	class PmdModel
	{
	public:
		float version;
		PmdHeader header;
		std::vector<PmdVertex> vertices;
		std::vector<uint16_t> indices;
		std::vector<PmdMaterial> materials;
		std::vector<PmdBone> bones;
		std::vector<PmdIk> iks;
		std::vector<PmdFace> faces;
		std::vector<uint16_t> faces_indices;
		std::vector<PmdBoneDispName> bone_disp_name;
		std::vector<PmdBoneDisp> bone_disp;
		std::vector<std::string> toon_filenames;
		std::vector<PmdRigidBody> rigid_bodies;
		std::vector<PmdConstraint> constraints;

		static std::unique_ptr<PmdModel> LoadFromFile(const char *filename)
		{
			std::wstring filename_wstring;
			oguna::EncodingConverter::Cp932ToUtf16(filename, static_cast<int>(strnlen(filename, 4096)), &filename_wstring);
			std::ifstream stream(filename_wstring, std::ios::binary);
			if (stream.fail())
			{
				std::cerr << "could not open \"" << filename << "\"" << std::endl;
				return nullptr;
			}
			auto result = LoadFromStream(&stream);
			stream.close();
			return result;
		}

		/// ファイルからPmdModelを生成する
		static std::unique_ptr<PmdModel> LoadFromStream(std::ifstream *stream)
		{
			auto result = std::make_unique<PmdModel>();
			char buffer[100];

			// magic
			char magic[3];
			stream->read(magic, 3);
			if (magic[0] != 'P' || magic[1] != 'm' || magic[2] != 'd')
			{
				std::cerr << "invalid file" << std::endl;
				return nullptr;
			}

			// version
			stream->read((char*) &(result->version), sizeof(float));
			if (result ->version != 1.0f)
			{
				std::cerr << "invalid version" << std::endl;
				return nullptr;
			}

			// header
			result->header.Read(stream);

			// vertices
			uint32_t vertex_num;
			stream->read((char*) &vertex_num, sizeof(uint32_t));
			result->vertices.resize(vertex_num);
			for (uint32_t i = 0; i < vertex_num; i++)
			{
				result->vertices[i].Read(stream);
			}

			// indices
			uint32_t index_num;
			stream->read((char*) &index_num, sizeof(uint32_t));
			result->indices.resize(index_num);
			for (uint32_t i = 0; i < index_num; i++)
			{
				stream->read((char*) &result->indices[i], sizeof(uint16_t));
			}

			// materials
			uint32_t material_num;
			stream->read((char*) &material_num, sizeof(uint32_t));
			result->materials.resize(material_num);
			for (uint32_t i = 0; i < material_num; i++)
			{
				result->materials[i].Read(stream);
			}

			// bones
			uint16_t bone_num;
			stream->read((char*) &bone_num, sizeof(uint16_t));
			result->bones.resize(bone_num);
			for (uint32_t i = 0; i < bone_num; i++)
			{
				result->bones[i].Read(stream);
			}

			// iks
			uint16_t ik_num;
			stream->read((char*) &ik_num, sizeof(uint16_t));
			result->iks.resize(ik_num);
			for (uint32_t i = 0; i < ik_num; i++)
			{
				result->iks[i].Read(stream);
			}

			// faces
			uint16_t face_num;
			stream->read((char*) &face_num, sizeof(uint16_t));
			result->faces.resize(face_num);
			for (uint32_t i = 0; i < face_num; i++)
			{
				result->faces[i].Read(stream);
			}

			// face frames
			uint8_t face_frame_num;
			stream->read((char*) &face_frame_num, sizeof(uint8_t));
			result->faces_indices.resize(face_frame_num);
			for (uint32_t i = 0; i < face_frame_num; i++)
			{
				stream->read((char*) &result->faces_indices[i], sizeof(uint16_t));
			}

			// bone names
			uint8_t bone_disp_num;
			stream->read((char*) &bone_disp_num, sizeof(uint8_t));
			result->bone_disp_name.resize(bone_disp_num);
			for (uint32_t i = 0; i < bone_disp_num; i++)
			{
				result->bone_disp_name[i].Read(stream);
			}

			// bone frame
			uint32_t bone_frame_num;
			stream->read((char*) &bone_frame_num, sizeof(uint32_t));
			result->bone_disp.resize(bone_frame_num);
			for (uint32_t i = 0; i < bone_frame_num; i++)
			{
				result->bone_disp[i].Read(stream);
			}

			// english name
			bool english;
			stream->read((char*) &english, sizeof(char));
			if (english)
			{
				result->header.ReadExtension(stream);
				for (uint32_t i = 0; i < bone_num; i++)
				{
					result->bones[i].ReadExpantion(stream);
				}
				for (uint32_t i = 0; i < face_num; i++)
				{
					if (result->faces[i].type == pmd::FaceCategory::Base)
					{
						continue;
					}
					result->faces[i].ReadExpantion(stream);
				}
				for (uint32_t i = 0; i < result->bone_disp_name.size(); i++)
				{
					result->bone_disp_name[i].ReadExpantion(stream);
				}
			}

			// toon textures
			if (stream->peek() == std::ios::traits_type::eof())
			{
				result->toon_filenames.clear();
			}
			else {
				result->toon_filenames.resize(10);
				for (uint32_t i = 0; i < 10; i++)
				{
					stream->read(buffer, 100);
					result->toon_filenames[i] = std::string(buffer);
				}
			}

			// physics
			if (stream->peek() == std::ios::traits_type::eof())
			{
				result->rigid_bodies.clear();
				result->constraints.clear();
			}
			else {
				uint32_t rigid_body_num;
				stream->read((char*) &rigid_body_num, sizeof(uint32_t));
				result->rigid_bodies.resize(rigid_body_num);
				for (uint32_t i = 0; i < rigid_body_num; i++)
				{
					result->rigid_bodies[i].Read(stream);
				}
				uint32_t constraint_num;
				stream->read((char*) &constraint_num, sizeof(uint32_t));
				result->constraints.resize(constraint_num);
				for (uint32_t i = 0; i < constraint_num; i++)
				{
					result->constraints[i].Read(stream);
				}
			}

			if (stream->peek() != std::ios::traits_type::eof())
			{
				std::cerr << "there is unknown data" << std::endl;
			}

			return result;
		}
	};
}
