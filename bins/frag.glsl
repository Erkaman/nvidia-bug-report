#version 430

#define GLUINT_MAX 4294967295

#define NULL_NODE      (GLUINT_MAX - 0)
#define SUBDIVIDE_NODE (GLUINT_MAX - 1)
#define LEAF_NODE      (GLUINT_MAX - 2)

// node in the octree. 
struct OctNode {
  uint nodePtr;	 // pointer to children.
  uint brickPtr;
  
  uint xPosNeighbor;
  uint yPosNeighbor;
  uint zPosNeighbor;
  
  uint xNegNeighbor;
  uint yNegNeighbor;
  uint zNegNeighbor;  	
};

// octree buffer. 
layout(binding = 1, std430) buffer lol9
{
   OctNode octree[];
};

out vec4 outColor;

// fetch voxel from octree. 
void voxelFetch(out vec3 outColor)
{
	
uint nodeAddress = 0;
vec3 boxMin = vec3(0.0);
vec3 boxDim = vec3(1.0);
vec3 childIndex = vec3(0.0);
uint nodePointer = octree[nodeAddress].nodePtr;

vec3 voxelPos = vec3(0.5, 0.361, 0.5);
	
// traverse octree, searching for voxel. 
while (nodePointer != NULL_NODE && 
       nodePointer != LEAF_NODE && 
	   nodePointer != SUBDIVIDE_NODE 
	 
	   )
{
boxDim *= 0.5;
childIndex = step(boxMin + boxDim, voxelPos);
nodeAddress = octree[nodeAddress].nodePtr + uint(dot(childIndex, vec3
(1.0, 2.0, 4.0)));
boxMin += (boxDim * childIndex);
nodePointer = octree[nodeAddress].nodePtr;
}

// This below code(2 lines):
if(nodePointer == NULL_NODE) { outColor = vec3(0.2, 0,0); return;}
outColor = vec3(0, 0.2,0);return;

/*
 should have the same result as this code

  if(nodePointer == NULL_NODE) return vec4(0.2, 0,0, 1);
  else return vec4(0, 0.2,0, 1);

but in the first case, it is appearently impossible for vec4(0.2, 0,0, 1); to ever 
be returned. If I check the assembler output for the shader,
then the very last line is:

  MOV.F result_color0, {0, 0.200000003, 1, 0}.xyxz;

so it is impossible for  vec4(0.2, 0,0, 1) to be returned with
the compiled code. but in the second case(the second code), you DO get the expected
result, and vec4(0.2, 0,0, 1) can be returned, as it ought to be. 

*/
}

void main()
{
	vec3 c;
	voxelFetch(c);
	outColor = vec4(c, 1.0);
}
