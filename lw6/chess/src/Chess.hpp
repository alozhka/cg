#pragma once

#include <graphics/Model.hpp>
#include <graphics/ObjLoader.hpp>
#include <graphics/shaders/ShaderProgram.hpp>
#include <graphics/textures/TextureCache.hpp>

#include <glm/glm.hpp>
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
		bool alive = true;
	};

	struct Move
	{
		int fromFile;
		int fromRank;
		int toFile;
		int toRank;
		int pieceIdx = -1;
		int capturedIdx = -1;
	};

	explicit Chess(TextureCache& textureCache)
		: m_board(ObjLoader::Load("assets/chess/board.obj", textureCache))
		, m_white{
			ObjLoader::Load("assets/chess/pawn.obj", textureCache),
			ObjLoader::Load("assets/chess/rook.obj", textureCache),
			ObjLoader::Load("assets/chess/knight.obj", textureCache),
			ObjLoader::Load("assets/chess/bishop.obj", textureCache),
			ObjLoader::Load("assets/chess/queen.obj", textureCache),
			ObjLoader::Load("assets/chess/king.obj", textureCache),
		}
		, m_black{
			ObjLoader::Load("assets/chess/pawn.obj", textureCache),
			ObjLoader::Load("assets/chess/rook.obj", textureCache),
			ObjLoader::Load("assets/chess/knight.obj", textureCache),
			ObjLoader::Load("assets/chess/bishop.obj", textureCache),
			ObjLoader::Load("assets/chess/queen.obj", textureCache),
			ObjLoader::Load("assets/chess/king.obj", textureCache),
		}
	{
		const Texture& blackTex = textureCache.Load("assets/chess/black_marble.jpg");
		for (auto& m : m_black)
		{
			m.SetDiffuseTexture(&blackTex);
		}
		SetupStartingPosition();
		SetupScholarsMate();
	}

	void Update(float dt)
	{
		if (m_currentMove >= m_moves.size())
		{
			return;
		}
		m_elapsed += dt;
		if (m_elapsed >= MOVE_DURATION + PAUSE_DURATION)
		{
			ApplyMove(m_moves[m_currentMove]);
			++m_currentMove;
			m_elapsed = 0.0f;
		}
	}

	void Draw(ShaderProgram& shader)
	{
		SetModelUniforms(shader, glm::mat4(1));
		m_board.Draw(shader);

		for (size_t i = 0; i < m_pieces.size(); ++i)
		{
			const Piece& p = m_pieces[i];
			if (!p.alive)
			{
				continue;
			}
			SetModelUniforms(shader, PieceTransform(p, static_cast<int>(i)));
			ModelOf(p).Draw(shader);
		}
	}

private:
	static constexpr int BOARD_FILES = 8;
	static constexpr float BOARD_SIZE = 1.9506f;
	static constexpr float SQUARE = BOARD_SIZE / BOARD_FILES;
	static constexpr float CENTER_OFFSET = (BOARD_FILES - 1) / 2.0f;

	static constexpr float MOVE_DURATION = 1;
	static constexpr float PAUSE_DURATION = 0.4;
	static constexpr float KNIGHT_LIFT = 0.12f;

	static glm::vec3 SquareCenter(float file, float rank)
	{
		const float x = (file - CENTER_OFFSET) * SQUARE;
		const float z = (rank - CENTER_OFFSET) * SQUARE;
		return { x, 0.0f, z };
	}

	glm::mat4 PieceTransform(const Piece& p, int index) const
	{
		glm::vec3 pos = SquareCenter(static_cast<float>(p.file), static_cast<float>(p.rank));

		if (m_currentMove < m_moves.size() && m_moves[m_currentMove].pieceIdx == index)
		{
			const Move& mv = m_moves[m_currentMove];
			const float t = SmoothStep(glm::clamp(m_elapsed / MOVE_DURATION, 0.0f, 1.0f));
			const glm::vec3 from = SquareCenter(static_cast<float>(mv.fromFile), static_cast<float>(mv.fromRank));
			const glm::vec3 to = SquareCenter(static_cast<float>(mv.toFile), static_cast<float>(mv.toRank));
			pos = glm::mix(from, to, t);

			if (p.type == PieceType::Knight)
			{
				pos.y += KNIGHT_LIFT * std::sin(t * glm::pi<float>());
			}
		}

		glm::mat4 m = glm::translate(glm::mat4(1), pos);
		if (p.color == Color::Black)
		{
			m *= BlackPieceRotation();
		}
		return m;
	}

	static float SmoothStep(float t)
	{
		return t * t * (3 - 2 * t);
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

	void ApplyMove(const Move& mv)
	{
		if (mv.capturedIdx >= 0)
		{
			m_pieces[mv.capturedIdx].alive = false;
		}
		Piece& p = m_pieces[mv.pieceIdx];
		p.file = mv.toFile;
		p.rank = mv.toRank;
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

	void SetupScholarsMate()
	{
		m_moves = {
			{ 4, 1, 4, 3 }, // e2 -> e4
			{ 4, 6, 4, 4 }, // e7 -> e5
			{ 5, 0, 2, 3 }, // Bf1 -> c4
			{ 1, 7, 2, 5 }, // Nb8 -> c6
			{ 3, 0, 7, 4 }, // Qd1 -> h5
			{ 6, 7, 5, 5 }, // Ng8 -> f6
			{ 7, 4, 5, 6 }, // Qxf7#
		};
		ResolveMoves();
	}

	void ResolveMoves()
	{
		std::array<std::array<int, BOARD_FILES>, BOARD_FILES> grid{};
		for (auto& row : grid)
		{
			row.fill(-1);
		}
		for (size_t i = 0; i < m_pieces.size(); ++i)
		{
			grid[m_pieces[i].file][m_pieces[i].rank] = i;
		}
		for (Move& mv : m_moves)
		{
			mv.pieceIdx = grid[mv.fromFile][mv.fromRank];
			mv.capturedIdx = grid[mv.toFile][mv.toRank];
			grid[mv.fromFile][mv.fromRank] = -1;
			grid[mv.toFile][mv.toRank] = mv.pieceIdx;
		}
	}

	Model m_board;
	std::array<Model, 6> m_white;
	std::array<Model, 6> m_black;
	std::array<Piece, 32> m_pieces;

	std::vector<Move> m_moves;
	size_t m_currentMove = 0;
	float m_elapsed = 0.0f;
};