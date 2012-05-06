#include "FaceManager.h"

#include "FaceSimple.h"
#include "FacePong.h"
#include "facepacman.h"
#include "facelife.h"
#include "facetix.h"
#include "faceanalog.h"
#include "FaceTemp.h"

//Here we instantiate all views
SimpleFace simpleFace;
PongFace pongFace;
PacManFace pacmanFace;
LifeFace lifeFace;
TixFace tixFace;
AnalogFace analogFace;
TempFace tempFace;

FaceManager::FaceManager() {
  faces[0] = &simpleFace;
  faces[1] = &pongFace;
  faces[2] = &pacmanFace;
  faces[3] = &lifeFace;
  faces[4] = &tixFace;
  faces[5] = &analogFace;
  faces[6] = &tempFace;
  currentFaceIdx = 0; // Need to initialise this
}

ClockFace* FaceManager::nextFace() {
  if(++currentFaceIdx >= FACE_COUNT)
    currentFaceIdx = 0;
  
  return faces[currentFaceIdx];
}

ClockFace* FaceManager::currentFace() const {
  return faces[currentFaceIdx];
}

