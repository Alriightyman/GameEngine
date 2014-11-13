import maya.cmds as cmds

def ExportToRTX():
    # set to one to turn debug messsages on
    debugOut = False
    # set to one to turn info messsages on
    infoMessages = False
    selection = cmds.ls(type="mesh",noIntermediate=True)
    
    print("calculating ...\n");
    # write index data
    for  obj in selection:
        cmds.select(obj,r=True)
        # write arrays	
        faceVertices = []
        vertexData = []
        normalData = []
        uvData = []
        indexedNormals = []
        indexedUVs = []
        hasUVs = True;
    
        faceToVertex = cmds.polyInfo(faceToVertex = True)
        size = len(faceToVertex)
        faces = size
        vertices = size*3
        iter = 0
        while iter < size:
            oneFaceToVertex = faceToVertex[iter].split()
            x = int(oneFaceToVertex[2])
            faceVertices.insert(iter*3,x)
            x = int(oneFaceToVertex[3])
            faceVertices.insert(iter*3+1,x)
            x = int(oneFaceToVertex[4])
            faceVertices.insert(iter*3+2,x)
            if (debugOut):
                print("VertexFace: " + str(oneFaceToVertex[1]) + " VertexIndex:" + str(oneFaceToVertex[2]) +", "+ str(oneFaceToVertex[3]) +", "+ str(oneFaceToVertex[4]))
            iter += 1
        size = cmds.polyEvaluate(v = True)
        
        numVertices = size
        indexedFaces = size
        vertices= size*3
        if (infoMessages):
            print ("Vertex count : "+ str(size));
        
        iter=0     
        while (iter < size):
            x  = obj + ".vtx[" + str(iter) + "]" 
            vtx = cmds.pointPosition(x, l= True)

            if (debugOut):
                print("Vertex " + str(iter) + ": " + str(vtx[0]) +", "+ str(vtx[1]) + ", "+ str(vtx[2]));
            vertexData.insert(iter*3,vtx[0])
            vertexData.insert(iter*3+1,vtx[1])
            vertexData.insert(iter*3+2,vtx[2])
            iter += 1
               
        
        iter = 0
        size = faces
        normal = []
        if (infoMessages):
            print ("Normal count : "+ str(size*3));
        
        currSelection = cmds.ls(sl=True)
        
        # extract normal data      
        while ( iter < size):
            if (infoMessages):
                print("index: " + str(iter) + "\n")
            temp = obj + ".vtxFace[" + str(faceVertices[iter*3]) + "][" + str(iter) +"]"
            cmds.select(temp,r=True)
            normal = cmds.polyNormalPerVertex(q=True, xyz=True)
            normalData.insert(iter*9,normal[0])
            normalData.insert(iter*9+1,normal[1])
            normalData.insert(iter*9+2,normal[2])
            if (debugOut):
                print("face " + str(iter) + ", normal 1: " + str(normalData[iter*9]) +", "+ str(normalData[iter*9+1]) +", "+ normalData[iter*9+2])
            temp = obj+".vtxFace["+str(faceVertices[iter*3+1])+"]["+str(iter)+"]"
            cmds.select(temp,r=True)
            normal = cmds.polyNormalPerVertex(q=True, xyz=True)
            normalData.insert(iter*9+3,normal[0])
            normalData.insert(iter*9+4,normal[1])
            normalData.insert(iter*9+5,normal[2])			
            
            if (debugOut):
                print("face " + str(iter) + ", normal 1: " + str(normalData[iter*9+3]) +", "+ str(normalData[iter*9+4]) +", "+ normalData[iter*9+5])

            temp = obj+".vtxFace["+str(faceVertices[iter*3+2])+"]["+str(iter)+"]"
            cmds.select(temp,r=True)
            normal = cmds.polyNormalPerVertex(q=True, xyz=True)
            normalData.insert(iter*9+6,normal[0])
            normalData.insert(iter*9+7,normal[1])
            normalData.insert(iter*9+8,normal[2])			
            
            if (debugOut):
                print("face " + str(iter) + ", normal 1: " + str(normalData[iter*9+6]) +", "+ str(normalData[iter*9+7]) +", "+ normalData[iter*9+8])
                       
            iter += 1
        	
        if (infoMessages):
            print ("UV count : "+ str(size*3));
        
        # extract uv data
        iter = 0
        fArray = []
        sArray = []        
        
        while (iter < size):        
            temp = obj+".vtxFace["+ str(faceVertices[iter*3]) + "][" + str(iter) + "]"
            cmds.select(temp,r=True)
            sArray = cmds.polyListComponentConversion(fromVertexFace=True,toUV=True)
            if not len(sArray):
                print("No UV's foun\n");
                hasUVs = False;
                break;
            
            fArray = cmds.polyEditUV(sArray[0],q=True)
            uvData.insert(iter*6,fArray[0])
            uvData.insert(iter*6+1,fArray[1])
            if (debugOut):
                print("face " + str(iter) + ", uv 1: " + str(uvData[iter*6]) + ", " + str(uvData[iter*6+1]))

            temp = obj+".vtxFace["+ str(faceVertices[iter*3+1]) + "]["+ str(iter) +"]"
            cmds.select(temp,r=True)
            sArray = cmds.polyListComponentConversion(fromVertexFace=True,toUV=True)
            fArray = cmds.polyEditUV(sArray[0],q=True)
            uvData.insert(iter*6+2,fArray[0])
            uvData.insert(iter*6+3,fArray[1])
            if (debugOut):
                print("face " + str(iter) + ", uv 1: " + str(uvData[iter*6+2]) + ", " + str(uvData[iter*6+3]))

            temp = obj+".vtxFace["+str(faceVertices[iter*3+2]) + "]["+ str(iter) +"]"
            cmds.select(temp,r=True)
            sArray = cmds.polyListComponentConversion(fromVertexFace=True,toUV=True)
            fArray = cmds.polyEditUV(sArray[0],q=True)
            uvData.insert(iter*6+4,fArray[0])
            uvData.insert(iter*6+5,fArray[1])
            if (debugOut):
                print("face " + str(iter) + ", uv 1: " + str(uvData[iter*6+4]) + ", " + str(uvData[iter*6+5]))

            iter += 1	
        
        cmds.select(currSelection,r=True)
			
       
        indexPos = 0
        iter = 0
        size = faces*3
        markedFaces = [0] * size
        
        print ("Number of faces: " + str(faces))
        print ("Number of vertices: " + str(size))
        # compare arrays:	
        print ("Calulating data: -------------------------------");	
 
        while ( iter < size):
            indexPos = faceVertices[iter]
            if (debugOut):
                print ("iter: "+ str(iter) + " Index pos: " + str(indexPos))
            if (markedFaces[indexPos] == 0):
                # if this face not yet written, just go on
                if (debugOut):
                    print (str(indexPos) + " write at indexPos : " + str(indexPos))
                markedFaces.insert(indexPos,1)
                indexedNormals.insert(indexPos*3,normalData[iter*3])
                indexedNormals.insert(indexPos*3+1,normalData[iter*3+1])
                indexedNormals.insert(indexPos*3+2,normalData[iter*3+2])
                indexedUVs.insert(indexPos*2,uvData[iter*2])
                indexedUVs.insert(indexPos*2+1,uvData[iter*2+1])

            else:
                # check if this vtx/normal/uv combination is the same as the one
                # already written. If not write a new vtx/normal/uv combination at the
                # end of the array
                if (debugOut):
                    print ("Check face iter: "+ str(iter) + ", vertex : " + str(indexPos))
                if (not((indexedNormals[indexPos*3]   == normalData[iter*3]) and
                      (indexedNormals[indexPos*3+1] == normalData[iter*3+1]) and
                      (indexedNormals[indexPos*3+2] == normalData[iter*3+2]) and 
                      (indexedUVs[indexPos*2]   == uvData[iter*2]) and
                      (indexedUVs[indexPos*2+1] == uvData[iter*2+1]) and
                      (vertexData[indexPos*3]   == vertexData[iter*3]) and
                      (vertexData[indexPos*3+1] == vertexData[iter*3+1]) and
                      (vertexData[indexPos*3+2] == vertexData[iter*3+2])
                    )):
                    
                    # also look in all the newly created vtx/normal/uv pairs for a match
                    for  i in range(numVertices,indexedFaces):
                        if (debugOut):
                            print ("inner Check face iter: "+ str(i) + ", vertex : " + str(indexPos))
                        if (((indexedNormals[i*3]   == normalData[iter*3]) and
                             (indexedNormals[i*3+1] == normalData[iter*3+1]) and
                             (indexedNormals[i*3+2] == normalData[iter*3+2])) and 
                            ((indexedUVs[i*2]   == uvData[iter*2]) and
                             (indexedUVs[i*2+1] == uvData[iter*2+1])) and
                            ((vertexData[i*3]   == vertexData[indexPos*3]) and
                             (vertexData[i*3+1] == vertexData[indexPos*3+1]) and
                             (vertexData[i*3+2] == vertexData[indexPos*3+2]) )):

                                faceVertices[iter] = i
                                if (debugOut):
                                    print("found in newly created vtx/normal/uv pair")
                                iter += 1
                                break
                                
                    if ( i < indexedFaces):
                        continue
                    if (infoMessages):
                        print (str(indexedFaces) + " Not found, writing index: "+ str(indexPos) +"/"+ str(iter) +" at vtxPos: "+ str(indexedFaces) )

                    indexedNormals.insert(indexedFaces*3,normalData[iter*3])
                    indexedNormals.insert(indexedFaces*3+1,normalData[iter*3+1])
                    indexedNormals.insert(indexedFaces*3+2,normalData[iter*3+2])

                    indexedUVs.insert(indexedFaces*2,uvData[iter*2])
                    indexedUVs.insert(indexedFaces*2+1,uvData[iter*2+1])

                    vertexData.insert(indexedFaces*3,vertexData[indexPos*3])
                    vertexData.insert(indexedFaces*3+1,vertexData[indexPos*3+1])
                    vertexData.insert(indexedFaces*3+2,vertexData[indexPos*3+2])
                    
                    faceVertices[iter] = indexedFaces
                    indexedFaces += 1

            iter += 1

        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
ExportToRTX()