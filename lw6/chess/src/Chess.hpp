#pragma once

#include <graphics/Model.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <vector>

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
		King,
		Count,
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
		SetModelUniforms(shader, glm::mat4(1.0f));
		m_board.Draw(shader);

		for (const auto& p : m_pieces)
		{
			SetModelUniforms(shader, PieceTransform(p));
			ModelOf(p).Draw(shader);
		}
	}

private:
	static constexpr float BOARD_SIZE = 1.9506f;
	static constexpr float SQUARE = BOARD_SIZE / 8.0f;

	glm::mat4 PieceTransform(const Piece& p) const
	{
		const float x = (static_cast<float>(p.file) - 3.5f) * SQUARE;
		const float z = (static_cast<float>(p.rank) - 3.5f) * SQUARE;
		glm::mat4 m(1.0f);
		m = glm::translate(m, glm::vec3(x, 0.0f, z));
		if (p.color == Color::Black)
		{
			m = glm::rotate(m, glm::pi<float>(), glm::vec3(0, 1, 0));
		}
		return m;
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
		static constexpr PieceType backRank[8] = {
			PieceType::Rook,   PieceType::Knight, PieceType::Bishop, PieceType::Queen,
			PieceType::King,   PieceType::Bishop, PieceType::Knight, PieceType::Rook,
		};
		for (int file = 0; file < 8; ++file)
		{
			m_pieces.push_back({ backRank[file],   Color::White, file, 0 });
			m_pieces.push_back({ PieceType::Pawn,  Color::White, file, 1 });
			m_pieces.push_back({ PieceType::Pawn,  Color::Black, file, 6 });
			m_pieces.push_back({ backRank[file],   Color::Black, file, 7 });
		}
	}

	Model m_board;
	std::array<Model, static_cast<size_t>(PieceType::Count)> m_white;
	std::array<Model, static_cast<size_t>(PieceType::Count)> m_black;
	std::vector<Piece> m_pieces;
};