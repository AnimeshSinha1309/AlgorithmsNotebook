#include "template.hpp"

#define AVG_CASE
#ifdef AVG_CASE
struct PushRelabel {
    struct Edge {
        ll from, to, cap, flow, index;
        Edge(ll dfrom, ll dto, ll dcap, ll dflow, ll dindex)
            : from(dfrom), to(dto), cap(dcap), flow(dflow), index(dindex) {
        }
    };

    ll size;
    vector<vector<Edge>> graph;
    vector<ll> excess;
    vector<ll> dist, active, count;
    queue<ll> q;

    PushRelabel(ll n)
        : size(n),
          graph(size),
          excess(size),
          dist(size),
          active(size),
          count(2 * size) {
    }
    void add_edge(ll from, ll to, ll cap) {
        graph[from].push_back(Edge(from, to, cap, 0, graph[to].size()));
        if (from == to)
            graph[from].back().index++;
        graph[to].push_back(Edge(to, from, 0, 0, graph[from].size() - 1));
    }
    void __enqueue(ll v) {
        if (!active[v] && excess[v] > 0) {
            active[v] = true;
            q.push(v);
        }
    }
    void __push(Edge &e) {
        ll amt = ll(min(excess[e.from], ll(e.cap - e.flow)));
        if (dist[e.from] <= dist[e.to] || amt == 0)
            return;
        e.flow += amt;
        graph[e.to][e.index].flow -= amt;
        excess[e.to] += amt;
        excess[e.from] -= amt;
        __enqueue(e.to);
    }
    void __gap(ll k) {
        for (ll v = 0; v < size; v++) {
            if (dist[v] < k)
                continue;
            count[dist[v]]--;
            dist[v] = max(dist[v], size + 1);
            count[dist[v]]++;
            __enqueue(v);
        }
    }
    void __relabel(ll v) {
        count[dist[v]]--;
        dist[v] = 2 * size;
        for (ll i = 0; i < (ll)graph[v].size(); i++)
            if (graph[v][i].cap - graph[v][i].flow > 0)
                dist[v] = min(dist[v], dist[graph[v][i].to] + 1);
        count[dist[v]]++;
        __enqueue(v);
    }
    void __discharge(ll v) {
        for (ll i = 0; excess[v] > 0 && i < (ll)graph[v].size(); i++)
            __push(graph[v][i]);
        if (excess[v] > 0) {
            if (count[dist[v]] == 1)
                __gap(dist[v]);
            else
                __relabel(v);
        }
    }
    ll max_flow(ll s, ll t) {
        count[0] = size - 1;
        count[size] = 1;
        dist[s] = size;
        active[s] = active[t] = true;
        for (ll i = 0; i < (ll)graph[s].size(); i++) {
            excess[s] += graph[s][i].cap;
            __push(graph[s][i]);
        }
        while (!q.empty()) {
            ll v = q.front();
            q.pop();
            active[v] = false;
            __discharge(v);
        }
        ll totflow = 0;
        for (ll i = 0; i < (ll)graph[s].size(); i++)
            totflow += graph[s][i].flow;
        return totflow;
    }
};
#else
struct Edge {
    int u, v;
    ll cap, flow;
    Edge() : u(0), v(0), cap(0), flow(0) {
    }
    Edge(int uu, int vv, ll ccap) : u(uu), v(vv), cap(ccap), flow(0) {
    }
};
struct Dinic {
    int N;
    vector<Edge> E;
    vector<vector<int>> g;
    vector<int> d, pt;
    Dinic(int NN) : N(NN), E(0), g(N), d(N), pt(N) {
    }
    void addEdge(int u, int v, ll cap, ll rcap = 0) {
        if (u != v) {
            E.emplace_back(Edge(u, v, cap));
            g[u].emplace_back(E.size() - 1);
            E.emplace_back(Edge(v, u, rcap));
            g[v].emplace_back(E.size() - 1);
        }
    }
    bool BFS(int S, int T) {
        queue<int> q({S});
        fill(d.begin(), d.end(), N + 1);
        d[S] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (u == T)
                break;
            for (int k : g[u]) {
                Edge &e = E[k];
                if (e.flow < e.cap && d[e.v] > d[e.u] + 1) {
                    d[e.v] = d[e.u] + 1;
                    q.emplace(e.v);
                }
            }
        }
        return d[T] != N + 1;
    }

    ll DFS(int u, int T, ll flow = -1) {
        if (u == T || flow == 0)
            return flow;
        for (int &i = pt[u]; i < (int)g[u].size(); ++i) {
            Edge &e = E[g[u][i]];
            Edge &oe = E[g[u][i] ^ 1];
            if (d[e.v] == d[e.u] + 1) {
                ll amt = e.cap - e.flow;
                if (flow != -1 && amt > flow)
                    amt = flow;
                if (ll pushed = DFS(e.v, T, amt)) {
                    e.flow += pushed;
                    oe.flow -= pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
    ll maxFlow(int S, int T) {
        ll total = 0;
        while (BFS(S, T)) {
            fill(pt.begin(), pt.end(), 0);
            while (ll flow = DFS(S, T))
                total += flow;
        }
        return total;
    }
};
#endif

struct HopcroftKarp {
    static const int INF = 1e9;
    int size_u, size_v, nil;
    vector<int> pair_u, pair_v, dist;
    vector<vector<int>> adjacency;

    bool __bfs() {
        queue<int> q;
        for (int u = 0; u < size_u; u++)
            if (pair_u[u] == nil)
                dist[u] = 0, q.push(u);
            else
                dist[u] = INF;
        dist[nil] = INF;
        while (not q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[u] >= dist[nil])
                continue;
            for (int v : adjacency[u])
                if (dist[pair_v[v]] == INF)
                    dist[pair_v[v]] = dist[u] + 1, q.push(pair_v[v]);
        }
        return dist[nil] != INF;
    }
    bool __dfs(int u) {
        if (u == nil)
            return true;
        for (int v : adjacency[u])
            if (dist[pair_v[v]] == dist[u] + 1)
                if (__dfs(pair_v[v])) {
                    pair_v[v] = u, pair_u[u] = v;
                    return true;
                }
        dist[u] = INF;
        return false;
    }

   public:
    HopcroftKarp(int u_size, int v_size) {
        nil = size_u = size_v = max(u_size, v_size);
        adjacency.resize(size_u + 1);
        dist.resize(size_u + 1);
        pair_u.resize(size_u + 1);
        pair_v.resize(size_v);
    }
    void add_edge(int u, int v) {
        adjacency[u].push_back(v);
    }
    int max_match() {
        fill(pair_u.begin(), pair_u.end(), nil);
        fill(pair_v.begin(), pair_v.end(), nil);
        int res = 0;
        while (__bfs())
            for (int u = 0; u < size_u; u++)
                if (pair_u[u] == nil && __dfs(u))
                    res++;
        return res;
    }
};