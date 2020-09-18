#include "gui.h"

Gui::Gui(std::array<WallConfig, 9> wallconfigs)
{
    // generate field blocks
    for (size_t y = 0; y < 3; ++y) {
        std::array<Block, 3> row;
        for (size_t x = 0; x < 3; ++x) {
            Block a_block(wallconfigs[y*3+x]);
            a_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
            row[x] = a_block;
        }
        m_blocks[y] = row;
    }

    // generate pieces
    for( size_t i = 0; i < m_whitePieces.size(); ++i ) {
        m_whitePieces[i]= Piece(whiteStart[i], WHITE);
    }

    for( size_t i = 0; i < m_blackPieces.size(); ++i ) {
        m_blackPieces[i]= Piece(blackStart[i], BLACK);
    }
}

std::array<Piece, 4> & Gui::getPieces(Player player){
    return player == Player::WHITE ? m_whitePieces : m_blackPieces;
}

void Gui::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for (auto row: m_blocks)
        for (auto b: row)
            target.draw(b, states);

    for (auto &p: m_whitePieces)
        target.draw(p, states);

    for (auto &p: m_blackPieces)
        target.draw(p, states);
}
