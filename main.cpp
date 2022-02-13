#include <bits/stdc++.h>
#include "Invoke.h"

using namespace std;

class MaxHeap {
    vector<int> heap_;

    inline static void sink(vector<int>& v, int i, int n)
    {
        int l = (i << 1) + 1;
        int r = (i << 1) + 2;

        int mi = i;
        if (l < n && v[mi] < v[l])
            mi = l;
        if (r < n && v[mi] < v[r])
            mi = r;
        if (mi != i) {
            std::swap(v[i], v[mi]);
            sink(v, mi, n);
        }
    }

    inline void sink(int i)
    {
        sink(heap_, i, heap_.size());
    }

    void swim(int i)
    {
        if (!i)
            return;
        int p = (i - 1) / 2;
        if (heap_[p] < heap_[i]) {
            std::swap(heap_[p], heap_[i]);
            swim(p);
        }
    }

    inline static void buildHeap(vector<int>& v)
    {
        for (int i = v.size() / 2; i >= 0; --i)
            sink(v, i, v.size());
    }

public:
    MaxHeap() = default;

    explicit MaxHeap(vector<int>&& v) : heap_(std::move(v))
    {
        buildHeap(heap_);
    }

    void push(int e)
    {
        heap_.push_back(e);
        swim(heap_.size() - 1);
    }

    void pop()
    {
        std::swap(heap_.front(), heap_.back());
        heap_.resize(heap_.size() - 1);
        sink(0);
    }

    [[nodiscard]] int top() const { return heap_.front(); }

    void printAll() const
    {
        for (int e : heap_)
            cout << e << ' ';
        cout << '\n';
    }

    inline static void heapSort(vector<int>& v)
    {
        buildHeap(v);

        for (int i = v.size() - 1; i; --i) {
            std::swap(v[0], v[i]);
            sink(v, 0, i);
        }
    }
};

void testVariant()
{
}

int main()
{
    vector<int> v{1,3,5,4,6,13,10,9,8,15,17};

    auto printV = [](const auto& v) {
        for (auto& e : v)
            cout << e << ' ';
        cout << '\n';
    };

    MaxHeap::heapSort(v);

    printV(v);

//    MaxHeap h(std::move(v));
//    h.printAll();

    return 0;
}
