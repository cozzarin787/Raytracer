#include "KdInterior.h"
#include "KdLeaf.h"

#define MAX_DEPTH 10

class KdTreeBuilder {
	private:
		int i = 0;

	public:
		KdTreeBuilder();

		Plane getPartitionPlane(Voxel v);
		std::vector<World::voxelObjectWrapper> getObjectsInVoxel(Voxel v, std::vector<World::voxelObjectWrapper> primitives);
		KdNode* getNode(Voxel v, std::vector<World::voxelObjectWrapper> primitives, int depth);

		std::vector<Object::intersectResult> rayThroughTree(KdNode* N, Ray r);

		std::string toString(KdNode* N);

		~KdTreeBuilder();
};
