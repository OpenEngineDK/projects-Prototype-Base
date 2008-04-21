SET( PROJECT_NAME "Prototype-Base")

# Project source code list
SET( PROTOTYPE_SOURCES
  # Add all the cpp source files here
  main.cpp
  GameFactory.cpp
  SnowTank.cpp
  SeanTank.cpp
  ShotManager.cpp
  Crosshair.cpp
  ClassicMovementHandler.cpp
  TankManager.cpp
  TankController.cpp
  GunManager.cpp
  LaserShot.cpp
  RapidLaserShot.cpp
  Landmine.cpp
  Shield.cpp
  LightningShot.cpp
  LaserGun.cpp
  RapidLaserGun.cpp
  LandmineLayer.cpp
  ShieldGenerator.cpp
  LightningGun.cpp
)

# todo get rid of this!@#!
# Include if using SDL under Mac OS X
IF(APPLE)
  SET(PROTOTYPE_SOURCES ${PROTOTYPE_SOURCES} ../../conf/mac/SDLMain.m)
ENDIF(APPLE)

# Project executable
ADD_EXECUTABLE(Prototype-Base ${PROTOTYPE_SOURCES})

# Project dependencies
TARGET_LINK_LIBRARIES(Prototype-Base 
  OpenEngine_Core
  OpenEngine_Logging
  OpenEngine_Display
  OpenEngine_Devices
  OpenEngine_Renderers
  OpenEngine_Renderers_OpenGL
  OpenEngine_Resources
  OpenEngine_Scene
  # Extensions
  Extensions_FixedTimeStepPhysics
  Extensions_SDL
  Extensions_OpenGLRenderer
  Extensions_OBJResource
  Extensions_TGAResource
)
