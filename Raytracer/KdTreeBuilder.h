#include "KdInterior.h"
#include "KdLeaf.h"

class KdTreeBuilder {
	private:
		int i = 0;

	public:
		KdTreeBuilder();

		Plane getPartitionPlane(Voxel v);
		std::vector<World::voxelObjectWrapper> getObjectsInVoxel(Voxel v, std::vector<World::voxelObjectWrapper> primitives);
		KdNode* getNode(Voxel v, std::vector<World::voxelObjectWrapper> primitives);

		std::vector<Object::intersectResult> rayThroughTree(KdNode* N, Ray r);

		~KdTreeBuilder();
};
