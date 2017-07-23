// KnapsackProblem.cpp : Defines the entry point for the console application.
//
#define CATCH_CONFIG_MAIN

#include <numeric>
#include <vector>
#include <catch.hpp>

struct Package {
	Package() : weight(0), value(0) {}
	Package(size_t w, size_t v) : weight(w), value(v) {}
	
	size_t weight;
	size_t value;
};

Package operator+(const Package& a, const Package& b) {
	return Package(a.weight + b.weight, a.value + b.value);
}



class Knapsack {
public:

	bool operator==(const Knapsack& k) const {
		if (this->value() != k.value()) return false;
		if (this->weight() != k.weight()) return false;

		// Check same items
		// TODO

		return true;
	}

	void addPackage(const Package& p) {
		packages_.push_back(p);
	}

	size_t weight() const {
		return sum().weight;
	}

	size_t value() const {
		return sum().value;
	}

private:
	std::vector<Package> packages_;

	Package sum() const {
		return std::accumulate(
			packages_.begin(),
			packages_.end(),
			Package()
		);
	}
};

Knapsack boundedPack(const std::vector<Package>& packages, const size_t max_weight) {
	const size_t itemCount = packages.size();
	
	// Initial knapsacks with 0 packages
	std::vector<Knapsack> vec(max_weight + 1);
	
	// Pack
	for (size_t i = 0; i < itemCount; ++i) {
		std::vector<Knapsack> newVec(max_weight + 1);
		const std::vector<Knapsack>& prevVec = vec;

		const Package& currentPackage = packages[i];
		for (size_t j = 0; j <= max_weight; ++j) {
			if (currentPackage.weight > j) {
				newVec[j] = prevVec[j];
			} else {
				if (prevVec[j].value() > (prevVec[j - currentPackage.weight].value() + currentPackage.value)) {
					newVec[j] = prevVec[j];
				} else {
					newVec[j] = prevVec[j - currentPackage.weight];
					newVec[j].addPackage(currentPackage);
				}
			}
		}
		vec = newVec;
	}

	return vec.back();
}

Knapsack unboundedPack(const std::vector<Package>& packages, const size_t max_weight) {
	const size_t itemCount = packages.size();

	// Initial knapsacks with 0 packages
	std::vector<Knapsack> vec(max_weight + 1);

	// Pack
	for (size_t i = 0; i < itemCount; ++i) {
		std::vector<Knapsack> newVec(max_weight + 1);
		const std::vector<Knapsack>& prevVec = vec;

		const Package& currentPackage = packages[i];
		for (size_t j = 0; j <= max_weight; ++j) {
			if (currentPackage.weight > j) {
				newVec[j] = prevVec[j];
			}
			else {
				Knapsack currentBag = prevVec[j];
				Knapsack prevBagWithThisItem = prevVec[j - currentPackage.weight];
				prevBagWithThisItem.addPackage(currentPackage);
				Knapsack currentBagWithThisItem = newVec[j - currentPackage.weight];
				currentBagWithThisItem.addPackage(currentPackage);

				// 3 way max choice
				if (currentBag.value() >= prevBagWithThisItem.value() && currentBag.value() >= currentBagWithThisItem.value()) {
					newVec[j] = currentBag;
				}
				else if (prevBagWithThisItem.value() >= currentBag.value() && prevBagWithThisItem.value() >= currentBagWithThisItem.value()) {
					newVec[j] = prevBagWithThisItem;
				}
				else {
					newVec[j] = currentBagWithThisItem;
				}
			}
		}
		vec = newVec;
	}

	return vec.back();
}

TEST_CASE("BoundedPack", "") {
	std::vector<Package> packages = {
		// weight, value
		Package(1, 1),
		Package(2, 3),
		Package(4, 5),
		Package(6, 3)
	};

	// 0 packages == empty sack
	REQUIRE(boundedPack(std::vector<Package>(), 15) == Knapsack());

	// packages all too heavy == empty sack
	REQUIRE(boundedPack(std::vector<Package>({
		Package(10, 1),
		Package(5, 1)
	}), 1) == Knapsack());

	Knapsack k = boundedPack(packages, 10);
	REQUIRE(k.value() == 9);
	REQUIRE(k.weight() == 7);

	std::vector<Package> revPackages(packages.rbegin(), packages.rend());
	k = boundedPack(revPackages, 10);
	REQUIRE(k.value() == 9);
	REQUIRE(k.weight() == 7);
}

TEST_CASE("UnboundedPack", "") {
	std::vector<Package> packages = {
		// weight, value
		Package(1, 1),
		Package(2, 3),
		Package(4, 5),
		Package(6, 3)
	};

	// 0 packages == empty sack
	REQUIRE(unboundedPack(std::vector<Package>(), 15) == Knapsack());

	// packages all too heavy == empty sack
	REQUIRE(unboundedPack(std::vector<Package>({
		Package(10, 1),
		Package(5, 1)
	}), 1) == Knapsack());

	Knapsack k = unboundedPack(packages, 10);
	REQUIRE(k.value() == 15);
	REQUIRE(k.weight() == 10);

	std::vector<Package> revPackages(packages.rbegin(), packages.rend());
	k = unboundedPack(revPackages, 10);
	REQUIRE(k.value() == 15);
	REQUIRE(k.weight() == 10);
}
