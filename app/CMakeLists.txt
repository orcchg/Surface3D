cmake_minimum_required(VERSION 3.4.1)

# -----------------------------------------------------------------------------
include_directories( src/main/cpp/3rd_party/contrib/clipper )
# clipper
set( TARGET_CLIPPER cipper )
set( SOURCE_CLIPPER src/main/cpp/3rd_party/contrib/clipper/clipper.cpp )
add_library( ${TARGET_CLIPPER} SHARED ${SOURCE_CLIPPER} )

# cms
include_directories( src/main/cpp/3rd_party/contrib/
set( TARGET_CMS cms )
set( SOURCE_CMS src/main/cpp/3rd_party/contrib/ )
# TODO

# ConvertUTF
include_directories( src/main/cpp/3rd_party/contrib/ConvertUTF )
set( TARGET_CONVERTUTF ConvertUTF )
set( SOURCE_CONVERTUTF src/main/cpp/3rd_party/contrib/ConvertUTF/ConvertUTF.c )
add_library( ${TARGET_CONVERTUTF} SHARED ${SOURCE_CONVERTUTF} )

# irrXML
include_directories( src/main/cpp/3rd_party/contrib/irrXML )
set( TARGET_IRRXML irrXML )
set( SOURCE_IRRXML src/main/cpp/3rd_party/contrib/irrXML.cpp )
add_library( ${TARGET_IRRXML} SHARED ${SOURCE_IRRXML} )

# jasper
include_directories( src/main/cpp/3rd_party/contrib/
set( TARGET_JASPER jasper )
set( SOURCE_JASPER
    src/main/cpp/3rd_party/contrib/
)
# TODO

# jpeg
# TODO

# mng
include_directories( src/main/cpp/3rd_party/contrib/
set( TARGET_MNG mng )
set( SOURCE_MNG
    src/main/cpp/3rd_party/contrib/mng/
)
# TODO

# png
include_directories( src/main/cpp/3rd_party/contrib/
set( TARGET_PNG png )
set( SOURCE_PNG
    src/main/cpp/3rd_party/contrib/png/
)
# TODO

# poly2tri
include_directories(
    src/main/cpp/3rd_party/contrib/poly2tri/poly2tri
    src/main/cpp/3rd_party/contrib/poly2tri/poly2tri/common
    src/main/cpp/3rd_party/contrib/poly2tri/poly2tri/sweep
)
set( TARGET_POLY2TRI poly2tri )
set( SOURCE_POLY2TRI
    src/main/cpp/3rd_party/contrib/poly2tri/poly2tri/common/shapes.cpp
    src/main/cpp/3rd_party/contrib/poly2tri/poly2tri/sweep/advancing_front.cpp
    src/main/cpp/3rd_party/contrib/poly2tri/poly2tri/sweep/sweep_context.cpp
)
add_library( ${TARGET_POLY2TRI} SHARED ${SOURCE_POLY2TRI} )

# tiff
include_directories( src/main/cpp/3rd_party/contrib/
set( TARGET_TIFF tiff )
set( SOURCE_TIFF
    src/main/cpp/3rd_party/contrib/
)
# TODO

# unzip
include_directories( src/main/cpp/3rd_party/contrib/unzip )
set( TARGET_UNZIP unzip )
set( SOURCE_UNZIP
    src/main/cpp/3rd_party/contrib/unzip/ioapi.c
    src/main/cpp/3rd_party/contrib/unzip/unzip.c
)
add_library( ${TARGET_UNZIP} SHARED ${SOURCE_UNZIP} )
target_link_libraries( ${TARGET_UNZIP} zlib )

# zlib
set( TARGET_ZLIB zlib )
set( SOURCE_ZLIB
    src/main/cpp/3rd_party/contrib/zlib/adler32.c
    src/main/cpp/3rd_party/contrib/zlib/compress.c
    src/main/cpp/3rd_party/contrib/zlib/crc32.c
    src/main/cpp/3rd_party/contrib/zlib/deflate.c
    src/main/cpp/3rd_party/contrib/zlib/gzclose.c
    src/main/cpp/3rd_party/contrib/zlib/gzlib.c
    src/main/cpp/3rd_party/contrib/zlib/gzread.c
    src/main/cpp/3rd_party/contrib/zlib/gzwrite.c
    src/main/cpp/3rd_party/contrib/zlib/infback.c
    src/main/cpp/3rd_party/contrib/zlib/inffast.c
    src/main/cpp/3rd_party/contrib/zlib/inflate.c
    src/main/cpp/3rd_party/contrib/zlib/inftrees.c
    src/main/cpp/3rd_party/contrib/zlib/trees.c
    src/main/cpp/3rd_party/contrib/zlib/uncompr.c
    src/main/cpp/3rd_party/contrib/zlib/zutil.c
)
add_library( ${TARGET_ZLIB} SHARED ${SOURCE_ZLIB} )

# assimp
include_directories( src/main/cpp/3rd_party/assimp )
set( TARGET_ASSIMP assimp )
set( SOURCE_ASSIMP
    src/main/cpp/3rd_party/assimp/src/3DSConverter.cpp
    src/main/cpp/3rd_party/assimp/src/3DSExporter.cpp
    src/main/cpp/3rd_party/assimp/src/3DSLoader.cpp
    src/main/cpp/3rd_party/assimp/src/ACLoader.cpp
    src/main/cpp/3rd_party/assimp/src/ASELoader.cpp
    src/main/cpp/3rd_party/assimp/src/ASEParser.cpp
    src/main/cpp/3rd_party/assimp/src/AssbinExporter.cpp
    src/main/cpp/3rd_party/assimp/src/AssbinLoader.cpp
    src/main/cpp/3rd_party/assimp/src/AssimpCExport.cpp
    src/main/cpp/3rd_party/assimp/src/Assimp.cpp
    src/main/cpp/3rd_party/assimp/src/AssimpPCH.cpp
    src/main/cpp/3rd_party/assimp/src/AssxmlExporter.cpp
    src/main/cpp/3rd_party/assimp/src/B3DImporter.cpp
    src/main/cpp/3rd_party/assimp/src/BaseImporter.cpp
    src/main/cpp/3rd_party/assimp/src/BaseProcess.cpp
    src/main/cpp/3rd_party/assimp/src/Bitmap.cpp
    src/main/cpp/3rd_party/assimp/src/BlenderBMesh.cpp
    src/main/cpp/3rd_party/assimp/src/BlenderDNA.cpp
    src/main/cpp/3rd_party/assimp/src/BlenderLoader.cpp
    src/main/cpp/3rd_party/assimp/src/BlenderModifier.cpp
    src/main/cpp/3rd_party/assimp/src/BlenderScene.cpp
    src/main/cpp/3rd_party/assimp/src/BlenderTessellator.cpp
    src/main/cpp/3rd_party/assimp/src/BVHLoader.cpp
    src/main/cpp/3rd_party/assimp/src/CalcTangentsProcess.cpp
    src/main/cpp/3rd_party/assimp/src/COBLoader.cpp
    src/main/cpp/3rd_party/assimp/src/ColladaExporter.cpp
    src/main/cpp/3rd_party/assimp/src/ColladaLoader.cpp
    src/main/cpp/3rd_party/assimp/src/ColladaParser.cpp
    src/main/cpp/3rd_party/assimp/src/ComputeUVMappingProcess.cpp
    src/main/cpp/3rd_party/assimp/src/ConvertToLHProcess.cpp
    src/main/cpp/3rd_party/assimp/src/CSMLoader.cpp
    src/main/cpp/3rd_party/assimp/src/DeboneProcess.cpp
    src/main/cpp/3rd_party/assimp/src/DefaultIOStream.cpp
    src/main/cpp/3rd_party/assimp/src/DefaultIOSystem.cpp
    src/main/cpp/3rd_party/assimp/src/DefaultLogger.cpp
    src/main/cpp/3rd_party/assimp/src/DXFLoader.cpp
    src/main/cpp/3rd_party/assimp/src/Exporter.cpp
    src/main/cpp/3rd_party/assimp/src/FBXAnimation.cpp
    src/main/cpp/3rd_party/assimp/src/FBXBinaryTokenizer.cpp
    src/main/cpp/3rd_party/assimp/src/FBXConverter.cpp
    src/main/cpp/3rd_party/assimp/src/FBXDeformer.cpp
    src/main/cpp/3rd_party/assimp/src/FBXDocument.cpp
    src/main/cpp/3rd_party/assimp/src/FBXDocumentUtil.cpp
    src/main/cpp/3rd_party/assimp/src/FBXImporter.cpp
    src/main/cpp/3rd_party/assimp/src/FBXMaterial.cpp
    src/main/cpp/3rd_party/assimp/src/FBXMeshGeometry.cpp
    src/main/cpp/3rd_party/assimp/src/FBXModel.cpp
    src/main/cpp/3rd_party/assimp/src/FBXNodeAttribute.cpp
    src/main/cpp/3rd_party/assimp/src/FBXParser.cpp
    src/main/cpp/3rd_party/assimp/src/FBXProperties.cpp
    src/main/cpp/3rd_party/assimp/src/FBXTokenizer.cpp
    src/main/cpp/3rd_party/assimp/src/FBXUtil.cpp
    src/main/cpp/3rd_party/assimp/src/FindDegenerates.cpp
    src/main/cpp/3rd_party/assimp/src/FindInstancesProcess.cpp
    src/main/cpp/3rd_party/assimp/src/FindInvalidDataProcess.cpp
    src/main/cpp/3rd_party/assimp/src/FixNormalsStep.cpp
    src/main/cpp/3rd_party/assimp/src/GenFaceNormalsProcess.cpp
    src/main/cpp/3rd_party/assimp/src/GenVertexNormalsProcess.cpp
    src/main/cpp/3rd_party/assimp/src/HMPLoader.cpp
    src/main/cpp/3rd_party/assimp/src/IFCBoolean.cpp
    src/main/cpp/3rd_party/assimp/src/IFCCurve.cpp
    src/main/cpp/3rd_party/assimp/src/IFCGeometry.cpp
    src/main/cpp/3rd_party/assimp/src/IFCLoader.cpp
    src/main/cpp/3rd_party/assimp/src/IFCMaterial.cpp
    src/main/cpp/3rd_party/assimp/src/IFCOpenings.cpp
    src/main/cpp/3rd_party/assimp/src/IFCProfile.cpp
    src/main/cpp/3rd_party/assimp/src/IFCReaderGen.cpp
    src/main/cpp/3rd_party/assimp/src/IFCUtil.cpp
    src/main/cpp/3rd_party/assimp/src/Importer.cpp
    src/main/cpp/3rd_party/assimp/src/ImporterRegistry.cpp
    src/main/cpp/3rd_party/assimp/src/ImproveCacheLocality.cpp
    src/main/cpp/3rd_party/assimp/src/IRRLoader.cpp
    src/main/cpp/3rd_party/assimp/src/IRRMeshLoader.cpp
    src/main/cpp/3rd_party/assimp/src/IRRShared.cpp
    src/main/cpp/3rd_party/assimp/src/JoinVerticesProcess.cpp
    src/main/cpp/3rd_party/assimp/src/LimitBoneWeightsProcess.cpp
    src/main/cpp/3rd_party/assimp/src/LWOAnimation.cpp
    src/main/cpp/3rd_party/assimp/src/LWOBLoader.cpp
    src/main/cpp/3rd_party/assimp/src/LWOLoader.cpp
    src/main/cpp/3rd_party/assimp/src/LWOMaterial.cpp
    src/main/cpp/3rd_party/assimp/src/LWSLoader.cpp
    src/main/cpp/3rd_party/assimp/src/MakeVerboseFormat.cpp
    src/main/cpp/3rd_party/assimp/src/MaterialSystem.cpp
    src/main/cpp/3rd_party/assimp/src/MD2Loader.cpp
    src/main/cpp/3rd_party/assimp/src/MD3Loader.cpp
    src/main/cpp/3rd_party/assimp/src/MD5Loader.cpp
    src/main/cpp/3rd_party/assimp/src/MD5Parser.cpp
    src/main/cpp/3rd_party/assimp/src/MDCLoader.cpp
    src/main/cpp/3rd_party/assimp/src/MDLLoader.cpp
    src/main/cpp/3rd_party/assimp/src/MDLMaterialLoader.cpp
    src/main/cpp/3rd_party/assimp/src/MS3DLoader.cpp
    src/main/cpp/3rd_party/assimp/src/NDOLoader.cpp
    src/main/cpp/3rd_party/assimp/src/NFFLoader.cpp
    src/main/cpp/3rd_party/assimp/src/ObjExporter.cpp
    src/main/cpp/3rd_party/assimp/src/ObjFileImporter.cpp
    src/main/cpp/3rd_party/assimp/src/ObjFileMtlImporter.cpp
    src/main/cpp/3rd_party/assimp/src/ObjFileParser.cpp
    src/main/cpp/3rd_party/assimp/src/OFFLoader.cpp
    src/main/cpp/3rd_party/assimp/src/OgreBinarySerializer.cpp
    src/main/cpp/3rd_party/assimp/src/OgreImporter.cpp
    src/main/cpp/3rd_party/assimp/src/OgreMaterial.cpp
    src/main/cpp/3rd_party/assimp/src/OgreStructs.cpp
    src/main/cpp/3rd_party/assimp/src/OgreXmlSerializer.cpp
    src/main/cpp/3rd_party/assimp/src/OptimizeGraph.cpp
    src/main/cpp/3rd_party/assimp/src/OptimizeMeshes.cpp
    src/main/cpp/3rd_party/assimp/src/PlyExporter.cpp
    src/main/cpp/3rd_party/assimp/src/PlyLoader.cpp
    src/main/cpp/3rd_party/assimp/src/PlyParser.cpp
    src/main/cpp/3rd_party/assimp/src/PostStepRegistry.cpp
    src/main/cpp/3rd_party/assimp/src/PretransformVertices.cpp
    src/main/cpp/3rd_party/assimp/src/ProcessHelper.cpp
    src/main/cpp/3rd_party/assimp/src/Q3BSPFileImporter.cpp
    src/main/cpp/3rd_party/assimp/src/Q3BSPFileParser.cpp
    src/main/cpp/3rd_party/assimp/src/Q3BSPZipArchive.cpp
    src/main/cpp/3rd_party/assimp/src/Q3DLoader.cpp
    src/main/cpp/3rd_party/assimp/src/RawLoader.cpp
    src/main/cpp/3rd_party/assimp/src/RemoveComments.cpp
    src/main/cpp/3rd_party/assimp/src/RemoveRedundantMaterials.cpp
    src/main/cpp/3rd_party/assimp/src/RemoveVCProcess.cpp
    src/main/cpp/3rd_party/assimp/src/ScaleToUnitBoxProcess.cpp
    src/main/cpp/3rd_party/assimp/src/SceneCombiner.cpp
    src/main/cpp/3rd_party/assimp/src/ScenePreprocessor.cpp
    src/main/cpp/3rd_party/assimp/src/SGSpatialSort.cpp
    src/main/cpp/3rd_party/assimp/src/SkeletonMeshBuilder.cpp
    src/main/cpp/3rd_party/assimp/src/SMDLoader.cpp
    src/main/cpp/3rd_party/assimp/src/SortByPTypeProcess.cpp
    src/main/cpp/3rd_party/assimp/src/SpatialSort.cpp
    src/main/cpp/3rd_party/assimp/src/SplitByBoneCountProcess.cpp
    src/main/cpp/3rd_party/assimp/src/SplitLargeMeshes.cpp
    src/main/cpp/3rd_party/assimp/src/StandardShapes.cpp
    src/main/cpp/3rd_party/assimp/src/STEPFileEncoding.cpp
    src/main/cpp/3rd_party/assimp/src/STEPFileReader.cpp
    src/main/cpp/3rd_party/assimp/src/STLExporter.cpp
    src/main/cpp/3rd_party/assimp/src/STLLoader.cpp
    src/main/cpp/3rd_party/assimp/src/Subdivision.cpp
    src/main/cpp/3rd_party/assimp/src/TargetAnimation.cpp
    src/main/cpp/3rd_party/assimp/src/TerragenLoader.cpp
    src/main/cpp/3rd_party/assimp/src/TextureTransform.cpp
    src/main/cpp/3rd_party/assimp/src/TriangulateProcess.cpp
    src/main/cpp/3rd_party/assimp/src/UnrealLoader.cpp
    src/main/cpp/3rd_party/assimp/src/ValidateDataStructure.cpp
    src/main/cpp/3rd_party/assimp/src/VertexTriangleAdjacency.cpp
    src/main/cpp/3rd_party/assimp/src/XFileExporter.cpp
    src/main/cpp/3rd_party/assimp/src/XFileImporter.cpp
    src/main/cpp/3rd_party/assimp/src/XFileParser.cpp
    src/main/cpp/3rd_party/assimp/src/XGLLoader.cpp
)
add_library( ${TARGET_ASSIMP} SHARED ${SOURCE_ASSIMP} )
target_link_libraries( ${TARGET_ASSIMP} irrXML ConvertUTF clipper poly2tri unzip zlib )

# -----------------------------------------------------------------------------
# Surface3D
include_directories(
    src/main/cpp/include
    src/main/cpp/include/nativeObject
    src/main/cpp/include/utils

set( TARGET_SURFACE3D Surface3D )
set( SOURCE_SURFACE3D
    src/main/cpp/AssetStorage.cpp
    src/main/cpp/AsyncContext.cpp
    src/main/cpp/EGLConfigChooser.cpp
    src/main/cpp/jni_asyncContext.cpp
    src/main/cpp/include/nativeObject/jni_nativeObject.cpp
    src/main/cpp/include/nativeObject/NativeObject.cpp
    src/main/cpp/include/nativeObject/Scene.cpp
    src/main/cpp/include/nativeObject/Signatures.cpp
    src/main/cpp/include/nativeObject/Texture.cpp
    src/main/cpp/utils/assimp_utils.cpp
    src/main/cpp/utils/illumination.cpp
    src/main/cpp/utils/material.cpp
    src/main/cpp/utils/rgbstruct.cpp
    src/main/cpp/utils/triangle.cpp
    src/main/cpp/utils/utils.cpp
    src/main/cpp/utils/vertex.cpp
)
add_library( ${TARGET_SURFACE3D} SHARED ${SOURCE_SURFACE3D} )
target_link_libraries( ${TARGET_SURFACE3D} log dl z png assimp android EGL GLESv1_CM )

