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


// Returns a billboard matrix:
//
//   A model matrix you can use with a vertex shader make an object always
//   face the camera. Useful for rendering planes containing things like
//   text or UI type elements, but that are placed within the 3d world.
//   This way they will change position as the camera moves, but alway face
//   the camera (never rotate).
//
//   What is happening here is the rotation portion of the supplied modelView
//   matrix (the upper 3x3 portion) is set back to identity. (the 1.0's and 0.0s).
//   This means that there is no rotation applied (the local coordinate frame
//   is equal to the camera's coordinate frame.

static ofMatrix4x4 getBillboardMatrix( const ofMatrix4x4& _modelViewMatrix )
{
    ofVec4f row0 = _modelViewMatrix.getRowAsVec4f( 0 );
    ofVec4f row1 = _modelViewMatrix.getRowAsVec4f( 1 );
    ofVec4f row2 = _modelViewMatrix.getRowAsVec4f( 2 );
    ofVec4f row3 = _modelViewMatrix.getRowAsVec4f( 3 );

    ofMatrix4x4 billboardMatrix( 1.0f, 0.0f, 0.0f, row0.w,
                                 0.0f, 1.0f, 0.0f, row1.w,
                                 0.0f, 0.0f, 1.0f, row2.w,
                                 row3.x, row3.y, row3.z, row3.w );

    return billboardMatrix;
} 

