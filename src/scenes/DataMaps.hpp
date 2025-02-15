#pragma once

#include "../components/GameContextComponents.hpp"

#include <queue>

namespace game::level{

using ct = cellType_t;


inline const wlLevelGrid TestLevel = {
	{ ct::WALL,	 ct::WALL,	ct::WALL,	ct::WALL,  ct::WALL },
	{ ct::START,	 ct::FLOOR, ct::FLOOR,	ct::FLOOR, ct::WALL },
	{ ct::WALL,	 ct::WALL,	ct::WALL,	ct::FLOOR, ct::WALL },
	{ ct::WALL,	 ct::FLOOR, ct::FLOOR,	ct::FLOOR, ct::WALL },
	{ ct::WALL,	 ct::END,	ct::WALL,	ct::WALL,  ct::WALL },
};

inline std::vector<wlCellPos> findPath( const wlLevelGrid& level, const wlCellPos startPath, const wlCellPos endPath ) {
	std::queue<wlCellPos> q;
	std::vector visited( level.size(), std::vector<bool>( level[0].size(), false ) );
	std::vector parent( level.size(), std::vector<wlCellPos>( level[0].size(), { -1, -1 } ) );

	static const std::vector<wlCellPos> directions = {
		{ -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

	q.push( startPath );
	visited[startPath.first][startPath.second] = true;

	bool found = false;

	while ( !q.empty() ) {
		const auto [row, col] = q.front();
		q.pop();

		if ( std::make_pair( row, col ) == endPath ) {
			found = true;
			break;
		}

		for ( const auto& dir : directions ) {
			const int32_t newRow = row + dir.first;
			const int32_t newCol = col + dir.second;

			if ( newRow >= 0 && newRow < level.size() && newCol >= 0 && newCol < level[newRow].size() ) {
				if ( !visited[newRow][newCol] && ( level[newRow][newCol] == ct::FLOOR || level[newRow][newCol] == ct::END ) ) {
					visited[newRow][newCol] = true;
					parent[newRow][newCol] = { row, col };
					q.push( { newRow, newCol } );
				}
			}
		}
	}

	std::vector<wlCellPos> path;
	if ( found ) {
		wlCellPos curr = endPath;
		while ( curr != startPath ) {
			path.push_back( curr );
			curr = parent[curr.first][curr.second];
		}
		path.push_back( startPath );
		std::ranges::reverse( path );
	}

	path.insert( path.cbegin(), startPath );
	path.push_back( endPath );

	return path;
}

inline std::vector<wlCellPos> findPath( const wlLevelGrid& level ) {
	wlCellPos	start;
	wlCellPos	end;
	bool		foundStart = false;
	bool		foundEnd = false;

	for ( int32_t row = 0; row < level.size(); ++row ) {
		for ( int32_t col = 0; col < level[row].size(); ++col ) {
			if ( level[row][col] == ct::START ) {
				start = { row, col };
				foundStart = true;
			}
			if ( level[row][col] == ct::END ) {
				end = { row, col };
				foundEnd = true;
			}
		}
	}

	return findPath( level, start, end );
}

};	// namespace game::level
