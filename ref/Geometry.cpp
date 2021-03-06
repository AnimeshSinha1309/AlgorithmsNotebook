#include "template.hpp"

class Point {
   public:
    typedef long long coord_t; coord_t x, y;
    Point(coord_t coord_x = 0, coord_t coord_y = 0) { this->x = coord_x; this->y = coord_y; }
    Point(pair<coord_t, coord_t> coord) { this->x = coord.first; this->y = coord.second; }
    friend bool operator<(const Point &a, const Point &b) { return (a.x != b.x) ? a.x < b.x : a.y < b.y; }
    friend bool operator==(const Point &a, const Point &b) { return (a.x == b.x) && (a.y == b.y); }
    friend istream &operator>>(istream &in, Point &p) { in >> p.x >> p.y; return in; }
    friend ostream &operator<<(ostream &out, Point &p) { out << p.x << " " << p.y; return out; }
    static coord_t area(const Point &a, const Point &b, const Point &c) { return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y); }; // Area function: area < 0 = clockwise, area > 0 counterclockwise
    static coord_t sq_dist(const Point &a, const Point &b) { return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y); }
    static coord_t cross(const Point &O, const Point &A, const Point &B) { return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x); }
    static coord_t dot(const Point &O, const Point &A, const Point &B) { return (A.x - O.x) * (B.x - O.x) + (A.y - O.y) * (B.y - O.y); }
    static coord_t area(const vector<Point> &polygon) {
        int n = polygon.size();
        coord_t ans = 0;
        for (int i = 0; i < n; i++)
            ans += polygon[i].x * polygon[(i + 1) % n].y - polygon[i].y * polygon[(i + 1) % n].x;
    }
    static vector<Point> convex_hull(vector<Point> &a) {
        if (a.size() <= 3) return a;
        int n = a.size(), k = 0; sort(a.begin(), a.end());
        vector<Point> result(2 * n);
        for (int i = 0; i < n; ++i) {
            while (k >= 2 && cross(result[k - 2], result[k - 1], a[i]) <= 0) k--;
            result[k++] = a[i];
        }
        for (int i = n - 1, t = k + 1; i > 0; --i) {
            while (k >= t && cross(result[k - 2], result[k - 1], a[i - 1]) <= 0) k--;
            result[k++] = a[i - 1];
        }
        result.resize(k - 1); return result;
    }
};