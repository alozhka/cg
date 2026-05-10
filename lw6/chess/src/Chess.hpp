#pragma once

#include <graphics/Model.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>

class Chess
{
public:
	enum class PieceType
	{
		Pawn = 0,
		Rook,
		Knight,
		Bishop,
		Queen,
		King
	};

	enum class Color
	{
		White,
		Black,
	};

	struct Piece
	{
		PieceType type;
		Color color;
		int file; // 0..7 (a..h)
		int rank; // 0..7 (1..8)
	};

	explicit Chess(TextureCache& textureCache)
		: m_board("assets/chess/board.obj", textureCache)
		, m_white{
			Model("assets/chess/pawn.obj", textureCache),
			Model("assets/chess/rook.obj", textureCache),
			Model("assets/chess/knight.obj", textureCache),
			Model("assets/chess/bishop.obj", textureCache),
			Model("assets/chess/queen.obj", textureCache),
			Model("assets/chess/king.obj", textureCache),
		}
		, m_black{
			Model("assets/chess/pawn.obj", textureCache),
			Model("assets/chess/rook.obj", textureCache),
			Model("assets/chess/knight.obj", textureCache),
			Model("assets/chess/bishop.obj", textureCache),
			Model("assets/chess/queen.obj", textureCache),
			Model("assets/chess/king.obj", textureCache),
		}
	{
		const Texture& blackTex = textureCache.Load("assets/chess/black_marble.jpg");
		for (auto& m : m_black)
		{
			m.SetDiffuseTexture(&blackTex);
		}
		SetupStartingPosition();
	}

	void Draw(ShaderProgram& shader)
	{
		SetModelUniforms(shader, glm::mat4(1));
		m_board.Draw(shader);

		for (const auto& p : m_pieces)
		{
			SetModelUniforms(shader, PieceTransform(p));
			ModelOf(p).Draw(shader);
		}
	}

private:
	static constexpr int BOARD_FILES = 8;
	static constexpr float BOARD_SIZE = 1.9506f;
	static constexpr float SQUARE = BOARD_SIZE / BOARD_FILES;
	static constexpr float CENTER_OFFSET = (BOARD_FILES - 1) / 2.0f;

	static glm::mat4 PieceTransform(const Piece& p)
	{
		const float x = (static_cast<float>(p.file) - CENTER_OFFSET) * SQUARE;
		const float z = (static_cast<float>(p.rank) - CENTER_OFFSET) * SQUARE;
		glm::mat4 m = glm::translate(glm::mat4(1), glm::vec3(x, 0, z));
		if (p.color == Color::Black)
		{
			m *= BlackPieceRotation();
		}
		return m;
	}

	static glm::mat4 BlackPieceRotation()
	{
		return glm::rotate(glm::mat4(1), glm::pi<float>(), glm::vec3(0, 1, 0));
	}

	Model& ModelOf(const Piece& p)
	{
		auto& set = (p.color == Color::White) ? m_white : m_black;
		return set[static_cast<size_t>(p.type)];
	}

	static void SetModelUniforms(ShaderProgram& shader, const glm::mat4& model)
	{
		shader.SetUniformMat4("uModel", model);
		shader.SetUniformMat3("uNormalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
	}

	void SetupStartingPosition()
	{
		static constexpr PieceType backRank[BOARD_FILES] = {
			PieceType::Rook,
			PieceType::Knight,
			PieceType::Bishop,
			PieceType::Queen,
			PieceType::King,
			PieceType::Bishop,
			PieceType::Knight,
			PieceType::Rook,
		};
		size_t i = 0;
		for (int file = 0; file < BOARD_FILES; ++file)
		{
			m_pieces[i++] = { backRank[file], Color::White, file, 0 };
			m_pieces[i++] = { PieceType::Pawn, Color::White, file, 1 };
			m_pieces[i++] = { PieceType::Pawn, Color::Black, file, 6 };
			m_pieces[i++] = { backRank[file], Color::Black, file, 7 };
		}
	}

	Model m_board;
	std::array<Model, 6> m_white;
	std::array<Model, 6> m_black;
	std::array<Piece, 32> m_pieces;
};