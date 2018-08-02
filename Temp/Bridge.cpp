 v);
            fup[v] = min(fup[v], fup[to]);
            if (fup[to] > tin[v])
                bridges.insert(make_pair(v, to));
        }
    }
}

void find_bridges() {
    timer = 0;
    memset(visited, 0, sizeof visited);
    memset(tin, -1, sizeof tin);
    memset(fup, -1, sizeof fup);

    for (int i = 0; i < n; ++i) {
        if (!visited[i])
            dfs(i);
    }
}
