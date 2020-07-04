#include "graph.hpp"

vector<Edge> euclidean_mst(const vector<Point>& points) {
    assert(points.size() > 1);
    int n = points.size();
    vector<bool> used(n, false);
    vector<int> cat(n, 0); // closestAmongTaken
    vector<Edge> res;
    // start in points[0]
    for (uint p = 0; p < n; p++)
        cat[p] = 0;
    used[0] = true;
    for (int cnt = 0; cnt < n - 1; cnt++) {
        // pick closest
        int from = -1;
        int closestDist = INT_MAX;
        for (uint p = 0; p < n; p++) {
            if (used[p])
                continue;
            int d = distSq(points[p], points[cat[p]]);
            if (d < closestDist) {
                closestDist = d;
                from = p;
            }
        }
        assert(from != -1);
        res.push_back({points[cat[from]], points[from]});
        used[from] = true;
        // update for all with this one
        for(int p = 0; p < n; p++){
            if(used[p])
            continue;
            int d_old = distSq(points[p], points[cat[p]]);
            int d_new = distSq(points[p], points[from]);
            if(d_new < d_old)
                cat[p] = from;
        }
    }
    return res;
}