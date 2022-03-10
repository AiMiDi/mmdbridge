#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <ostream>

namespace vmd
{
	/// ボーンフレーム
	class VmdBoneFrame
	{
	public:
		/// ボーン名
		std::string name;
		/// フレーム番号
		int frame = 0;
		/// 位置
		float position[3] = { 0,0,0 };
		/// 回転
		float orientation[4] = { 0,0,0,1 };
		/// 補間曲線
		char interpolation[4][4][4];

		void Read(std::istream* stream)
		{
			char buffer[15];
			stream->read((char*) buffer, sizeof(char)*15);
			name = std::string(buffer);
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) position, sizeof(float)*3);
			stream->read((char*) orientation, sizeof(float)*4);
			stream->read((char*) interpolation, sizeof(char) * 4 * 4 * 4);
		}

		void Write(std::ostream* stream)
		{
			stream->write((char*)name.c_str(), sizeof(char) * 15);
			stream->write((char*)&frame, sizeof(int));
			stream->write((char*)position, sizeof(float) * 3);
			stream->write((char*)orientation, sizeof(float) * 4);
			stream->write((char*)interpolation, sizeof(char) * 4 * 4 * 4);
		}
		VmdBoneFrame(){}

		VmdBoneFrame(const VmdBoneFrame&) = delete;

		VmdBoneFrame(VmdBoneFrame&& other) noexcept :
			name(std::move(other.name)),
			frame(std::move(other.frame))			
		{
			std::swap(position, other.position);
			std::swap(orientation, other.orientation);
			std::swap(interpolation, other.interpolation);
		}

		~VmdBoneFrame() {}
	};

	/// 表情フレーム
	class VmdFaceFrame
	{
	public:
		/// 表情名
		std::string face_name;
		/// 表情の重み
		float weight= 0.f;
		/// フレーム番号
		uint32_t frame = 0;

		void Read(std::istream* stream)
		{
			char buffer[15];
			stream->read((char*) &buffer, sizeof(char) * 15);
			face_name = std::string(buffer);
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) &weight, sizeof(float));
		}

		void Write(std::ostream* stream)
		{
			stream->write((char*)face_name.c_str(), sizeof(char) * 15);
			stream->write((char*)&frame, sizeof(int));
			stream->write((char*)&weight, sizeof(float));
		}

		VmdFaceFrame() {}

		VmdFaceFrame(const VmdFaceFrame&) = delete;

		VmdFaceFrame(VmdFaceFrame&& other) noexcept :
			face_name(std::move(other.face_name)),
			weight(std::move(other.weight)),
			frame(std::move(other.frame)) {}
	};

	/// カメラフレーム
	class VmdCameraFrame
	{
	public:
		/// フレーム番号
		int frame = 0;
		/// 距離
		float distance = 0.f;
		/// 位置
		float position[3] = { 0,0,0 };
		/// 回転
		float orientation[3] = { 0,0,0 };
		/// 補間曲線
		char interpolation[6][4];
		/// 視野角
		float angle;
		/// 不明データ
		char unknown[3];

		void Read(std::istream *stream)
		{
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) &distance, sizeof(float));
			stream->read((char*) position, sizeof(float) * 3);
			stream->read((char*) orientation, sizeof(float) * 3);
			stream->read((char*) interpolation, sizeof(char) * 24);
			stream->read((char*) &angle, sizeof(float));
			stream->read((char*) unknown, sizeof(char) * 3);
		}

		void Write(std::ostream *stream)
		{
			stream->write((char*)&frame, sizeof(int));
			stream->write((char*)&distance, sizeof(float));
			stream->write((char*)position, sizeof(float) * 3);
			stream->write((char*)orientation, sizeof(float) * 3);
			stream->write((char*)interpolation, sizeof(char) * 24);
			stream->write((char*)&angle, sizeof(float));
			stream->write((char*)unknown, sizeof(char) * 3);
		}

		VmdCameraFrame() {}

		VmdCameraFrame(const VmdCameraFrame&) = delete;

		VmdCameraFrame(VmdCameraFrame&& other) noexcept :
			frame(std::move(other.frame)),
			distance(std::move(other.distance)),
			angle(std::move(other.angle)) {
			std::swap(position, other.position);
			std::swap(orientation, other.orientation);
			std::swap(interpolation, other.interpolation);
			std::swap(unknown, other.unknown);
		}
	};

	/// ライトフレーム
	class VmdLightFrame
	{
	public:
		/// フレーム番号
		int frame;
		/// 色
		float color[3];
		/// 位置
		float position[3];

		void Read(std::istream* stream)
		{
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) color, sizeof(float) * 3);
			stream->read((char*) position, sizeof(float) * 3);
		}

		void Write(std::ostream* stream)
		{
			stream->write((char*)&frame, sizeof(int));
			stream->write((char*)color, sizeof(float) * 3);
			stream->write((char*)position, sizeof(float) * 3);
		}

		VmdLightFrame() {}

		VmdLightFrame(const VmdLightFrame&) = delete;

		VmdLightFrame(VmdLightFrame&& other) noexcept :
			frame(std::move(other.frame)) {
			std::swap(position, other.position);
			std::swap(color, other.color);
		}
	};

	/// IKの有効無効
	class VmdIkEnable
	{
	public:
		std::string ik_name;
		bool enable;
	};

	/// IKフレーム
	class VmdIkFrame
	{
	public:
		int frame = 0;
		bool display = true;
		std::vector<VmdIkEnable> ik_enable;

		void Read(std::istream *stream)
		{
			char buffer[20];
			stream->read((char*) &frame, sizeof(int));
			stream->read((char*) &display, sizeof(uint8_t));
			int ik_count;
			stream->read((char*) &ik_count, sizeof(int));
			ik_enable.resize(ik_count);
			for (int i = 0; i < ik_count; i++)
			{
				stream->read(buffer, 20);
				ik_enable[i].ik_name = std::string(buffer);
				stream->read((char*) &ik_enable[i].enable, sizeof(uint8_t));
			}
		}

		void Write(std::ostream *stream)
		{
			stream->write((char*)&frame, sizeof(int));
			stream->write((char*)&display, sizeof(uint8_t));
			int ik_count = static_cast<int>(ik_enable.size());
			stream->write((char*)&ik_count, sizeof(int));
			for (int i = 0; i < ik_count; i++)
			{
				const VmdIkEnable& ik_enable = this->ik_enable.at(i);
				stream->write(ik_enable.ik_name.c_str(), 20);
				stream->write((char*)&ik_enable.enable, sizeof(uint8_t));
			}
		}

		VmdIkFrame() {}

		VmdIkFrame(const VmdIkFrame&) = delete;

		VmdIkFrame(VmdIkFrame&& other) noexcept :
			frame(std::move(other.frame)),
			display(std::move(other.display)),
			ik_enable(std::move(other.ik_enable)) {}
	};

	/// VMDモーション
	class VmdMotion
	{
	public:
		/// モデル名
		std::string model_name;
		/// バージョン
		int version;
		/// ボーンフレーム
		std::vector<VmdBoneFrame> bone_frames;
		/// 表情フレーム
		std::vector<VmdFaceFrame> face_frames;
		/// カメラフレーム
		std::vector<VmdCameraFrame> camera_frames;
		/// ライトフレーム
		std::vector<VmdLightFrame> light_frames;
		/// IKフレーム
		std::vector<VmdIkFrame> ik_frames;

		static std::unique_ptr<VmdMotion> LoadFromFile(char const *filename)
		{
			std::ifstream stream(filename, std::ios::binary);
			auto result = LoadFromStream(&stream);
			stream.close();
			return result;
		}

		static std::unique_ptr<VmdMotion> LoadFromStream(std::ifstream *stream)
		{

			char buffer[30];
			auto result = std::make_unique<VmdMotion>();

			// magic and version
			stream->read((char*) buffer, 30);
			if (strncmp(buffer, "Vocaloid Motion Data", 20))
			{
				std::cerr << "invalid vmd file." << std::endl;
				return nullptr;
			}
			result->version = std::atoi(buffer + 20);

			// name
			stream->read(buffer, 20);
			result->model_name = std::string(buffer);

			// bone frames
			int bone_frame_num;
			stream->read((char*) &bone_frame_num, sizeof(int));
			result->bone_frames.resize(bone_frame_num);
			for (int i = 0; i < bone_frame_num; i++)
			{
				result->bone_frames[i].Read(stream);
			}

			// face frames
			int face_frame_num;
			stream->read((char*) &face_frame_num, sizeof(int));
			result->face_frames.resize(face_frame_num);
			for (int i = 0; i < face_frame_num; i++)
			{
				result->face_frames[i].Read(stream);
			}

			// camera frames
			int camera_frame_num;
			stream->read((char*) &camera_frame_num, sizeof(int));
			result->camera_frames.resize(camera_frame_num);
			for (int i = 0; i < camera_frame_num; i++)
			{
				result->camera_frames[i].Read(stream);
			}

			// light frames
			int light_frame_num;
			stream->read((char*) &light_frame_num, sizeof(int));
			result->light_frames.resize(light_frame_num);
			for (int i = 0; i < light_frame_num; i++)
			{
				result->light_frames[i].Read(stream);
			}

			// unknown2
			stream->read(buffer, 4);

			// ik frames
			if (stream->peek() != std::ios::traits_type::eof())
			{
				int ik_num;
				stream->read((char*) &ik_num, sizeof(int));
				result->ik_frames.resize(ik_num);
				for (int i = 0; i < ik_num; i++)
				{
					result->ik_frames[i].Read(stream);
				}
			}

			if (stream->peek() != std::ios::traits_type::eof())
			{
				std::cerr << "vmd stream has unknown data." << std::endl;
			}

			return result;
		}

		bool SaveToFile(const std::wstring& filename)
		{
			std::ofstream stream(filename.c_str(), std::ios::binary);
			auto result = SaveToStream(&stream);
			stream.close();
			return result;
		}

		bool SaveToStream(std::ofstream *stream)
		{
			std::string magic = "Vocaloid Motion Data 0002\0";
			magic.resize(30);

			// magic and version
			stream->write(magic.c_str(), 30);

			// name
			stream->write(model_name.c_str(), 20);

			// bone frames
			const int bone_frame_num = static_cast<int>(bone_frames.size());
			stream->write(reinterpret_cast<const char*>(&bone_frame_num), sizeof(int));
			for (int i = 0; i < bone_frame_num; i++)
			{
				bone_frames[i].Write(stream);
			}

			// face frames
			const int face_frame_num = static_cast<int>(face_frames.size());
			stream->write(reinterpret_cast<const char*>(&face_frame_num), sizeof(int));
			for (int i = 0; i < face_frame_num; i++)
			{
				face_frames[i].Write(stream);
			}

			// camera frames
			const int camera_frame_num = static_cast<int>(camera_frames.size());
			stream->write(reinterpret_cast<const char*>(&camera_frame_num), sizeof(int));
			for (int i = 0; i < camera_frame_num; i++)
			{
				camera_frames[i].Write(stream);
			}

			// light frames
			const int light_frame_num = static_cast<int>(light_frames.size());
			stream->write(reinterpret_cast<const char*>(&light_frame_num), sizeof(int));
			for (int i = 0; i < light_frame_num; i++)
			{
				light_frames[i].Write(stream);
			}

			// self shadow datas
			const int self_shadow_num = 0;
			stream->write(reinterpret_cast<const char*>(&self_shadow_num), sizeof(int));

			// ik frames
			const int ik_num = static_cast<int>(ik_frames.size());
			stream->write(reinterpret_cast<const char*>(&ik_num), sizeof(int));
			for (int i = 0; i < ik_num; i++)
			{
				ik_frames[i].Write(stream);
			}

			return true;
		}

		VmdMotion() {}

		VmdMotion(const VmdMotion&) = delete;

		VmdMotion(VmdMotion&& other) noexcept :
			model_name(std::move(other.model_name)),
			version(std::move(other.version)),
			bone_frames(std::move(other.bone_frames)),
			face_frames(std::move(other.face_frames)),
			camera_frames(std::move(other.camera_frames)),
			light_frames(std::move(other.light_frames)),
			ik_frames(std::move(other.ik_frames)) {}
	};
}
