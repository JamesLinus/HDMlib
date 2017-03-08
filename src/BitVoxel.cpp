/*
###################################################################################
#
# HDMlib - Data management library for hierarchical Cartesian data structure
#
# Copyright (c) 2014-2017 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
 */

///
/// @file  BitVoxel.cpp
/// @brief ビットボクセル圧縮/展開ライブラリ
///

#include <cstring>

#include "BitVoxel.h"

namespace BCMFileIO
{
	typedef unsigned int bitVoxelCell;

	BitVoxel::BitVoxel()
	{
	}

	BitVoxel::~BitVoxel()
	{
	}

	size_t BitVoxel::GetSize(const size_t voxelSize, const unsigned char bitWidth)
	{
		const unsigned char vox_per_cell = (sizeof(bitVoxelCell) * 8) / bitWidth;
		return (voxelSize / vox_per_cell + (voxelSize % vox_per_cell == 0 ? 0 : 1 ));
	}

	bitVoxelCell* BitVoxel::Compress( size_t* bitVoxelSize, const size_t voxelSize, const unsigned char* voxel, const unsigned char  bitWidth)
	{
		const unsigned char vox_per_cell = (sizeof(bitVoxelCell) * 8) / bitWidth;
		size_t bsz  = voxelSize / vox_per_cell + (voxelSize % vox_per_cell == 0 ? 0 : 1);

		bitVoxelCell* bitVoxel = new bitVoxelCell[bsz];
		memset(bitVoxel, 0, sizeof(bitVoxelCell) * bsz);

		unsigned char mask = 0;
		for(int i = 0; i < bitWidth; i++) mask += (1 << i);

		for(size_t i = 0; i < voxelSize; i++){
			size_t       cellIdx =  i / vox_per_cell;
			unsigned int bitIdx  = (i % vox_per_cell) * bitWidth;

			unsigned char c = voxel[i];

			bitVoxel[cellIdx] += (c & mask) << bitIdx;
		}

		*bitVoxelSize = bsz;
		return bitVoxel;

	}

	unsigned char* BitVoxel::Decompress( const size_t voxelSize, const bitVoxelCell* bitVoxel, const unsigned char  bitWidth)
	{
		const unsigned char vox_per_cell = (sizeof(bitVoxelCell) * 8) / bitWidth;

		unsigned char* voxel = new unsigned char[voxelSize];
		memset(voxel, 0, sizeof(unsigned char) * voxelSize);

		unsigned char mask = 0;
		for(int i = 0; i < bitWidth; i++) mask += (1 << i);

		for(size_t i = 0; i < voxelSize; i++){
			size_t       cellIdx =  i / vox_per_cell;
			unsigned int bitIdx  = (i % vox_per_cell) * bitWidth;

			voxel[i] = (bitVoxel[cellIdx] >> bitIdx) & mask;
		}

		return voxel;
	}


} // namespace BCMFileIO
