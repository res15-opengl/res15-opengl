#pragma once

// Filename: Billboard.h
// 
// Copyright © James Acres
// http://www.jamesacres.com
// http://github.com/jacres
// @jimmyacres
// 
// Created: Thu Apr  2 18:36:16 2015 (-0400)
// Last-Updated: Thu Apr  2 18:47:27 2015 (-0400)

static ofMatrix4x4 getBillboardMatrix( const ofMatrix4x4& _modelViewMatrix )
{
    ofVec4f row0 = _modelViewMatrix.getRowAsVec4f( 0 );
    ofVec4f row1 = _modelViewMatrix.getRowAsVec4f( 1 );
    ofVec4f row2 = _modelViewMatrix.getRowAsVec4f( 2 );
    ofVec4f row3 = _modelViewMatrix.getRowAsVec4f( 3 );

    ofMatrix4x4 billboardMatrix( 1.0f, 0.0f, 0.0f, row0.w,
                                 0.0f, 1.0f, 0.0f, row1.w,
                                 0.0f, 0.0f, 0.0f, row2.w,
                                 row3.x, row3.y, row3.z, row3.w );

    return billboardMatrix;
} 

