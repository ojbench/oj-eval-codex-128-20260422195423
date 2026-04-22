#include <bits/stdc++.h>
using namespace std;

static bool in_bounds(int r, int c) { return r>=0 && r<3 && c>=0 && c<3; }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<string> b(3);
    for (int i = 0; i < 3; ++i) {
        if (!(cin >> b[i])) return 0;
        if ((int)b[i].size() != 3) { cout << "invalid\n"; return 0; }
    }

    int cntX = 0, cntO = 0, cntDash = 0;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            char ch = b[r][c];
            if (ch == 'x') cntX++;
            else if (ch == 'o') cntO++;
            else if (ch == '-') cntDash++;
            else { cout << "invalid\n"; return 0; }
        }
    }

    auto lines = vector<array<pair<int,int>,3>>{
        // rows
        {{{0,0},{0,1},{0,2}}},
        {{{1,0},{1,1},{1,2}}},
        {{{2,0},{2,1},{2,2}}},
        // cols
        {{{0,0},{1,0},{2,0}}},
        {{{0,1},{1,1},{2,1}}},
        {{{0,2},{1,2},{2,2}}},
        // diags
        {{{0,0},{1,1},{2,2}}},
        {{{0,2},{1,1},{2,0}}}
    };

    auto getWins = [&](char p){
        vector<vector<pair<int,int>>> wins;
        for (auto &ln : lines) {
            bool ok = true;
            for (auto &cell : ln) if (b[cell.first][cell.second] != p) ok = false;
            if (ok) wins.push_back({ln.begin(), ln.end()});
        }
        return wins;
    };

    auto winsX = getWins('x');
    auto winsO = getWins('o');

    // Basic count validity
    if (!(cntX == cntO || cntX == cntO + 1)) {
        cout << "invalid\n";
        return 0;
    }

    // Both cannot win
    if (!winsX.empty() && !winsO.empty()) {
        cout << "invalid\n";
        return 0;
    }

    auto all_lines_intersection_contains = [&](const vector<vector<pair<int,int>>> &wins){
        if (wins.empty()) return false;
        set<pair<int,int>> inter;
        for (auto &cell : wins[0]) inter.insert(cell);
        for (size_t i = 1; i < wins.size(); ++i) {
            set<pair<int,int>> nxt;
            for (auto &cell : wins[i]) if (inter.count(cell)) nxt.insert(cell);
            inter.swap(nxt);
            if (inter.empty()) break;
        }
        // last move must be in intersection
        for (auto &p : inter) {
            if (in_bounds(p.first, p.second)) return true;
        }
        return false;
    };

    if (!winsX.empty()) {
        // X must have moved last
        if (cntX != cntO + 1) { cout << "invalid\n"; return 0; }
        // If multiple win lines, they must share the last-move cell
        if (winsX.size() >= 2 && !all_lines_intersection_contains(winsX)) {
            cout << "invalid\n"; return 0;
        }
        cout << "win\n"; return 0;
    }

    if (!winsO.empty()) {
        // O must have moved last
        if (cntX != cntO) { cout << "invalid\n"; return 0; }
        if (winsO.size() >= 2 && !all_lines_intersection_contains(winsO)) {
            cout << "invalid\n"; return 0;
        }
        cout << "lose\n"; return 0;
    }

    // No winner
    if (cntDash > 0) cout << "ongoing\n";
    else cout << "tie\n";
    return 0;
}

