#pragma once

// Graphics processing handler declarations
bool gfInitScene();
void gfDrawScene();
void gfCleanupScene();
void gfOnLMouseClick( int x, int y );
void gfOnRMouseClick( int x, int y );
void gfOnKeyDown( UINT key );
void gfOnKeyUp( UINT key );
