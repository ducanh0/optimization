/**
    chay 2.5 s lay trang thai tot nhat r local search
*/
#include <bits/stdc++.h>
using namespace std;

using namespace chrono;

class timer: high_resolution_clock {
    const time_point start_time;
public:
    timer(): start_time(now()) {}
    rep elapsed_time() const { return duration_cast<milliseconds>(now()-start_time).count(); } };

const int Ti = 2900, Titi = 2500;
timer t; /// create a timer object

int arr[10][10], N, T; /// input
int H = 0, best_s = 0, tmp_s = 0; /// do sau hien tai, diem so tot nhat, diem so hien tai
string best_t = "", tmp_t = ""; /// loi giai tot nhat, loi giai hien tai

int dx[] = {0, -1, 0, 1}, dy[] = {-1, 0, 1, 0};
char dir[] = {'L', 'U', 'R', 'D'};
int idx[] = {0, 1, 2, 3};

int pos_x, pos_y;   /// position of empty square
int bdX, bdY;

bool isInside(int x,int y){
    return 1 - (x < 0 || x >= N || y < 0 || y >= N);
}

bool isBitOn(int i,int j){  /// whether bit j of i equal 1 ?
    return ((i >> j) & 1);
}

bool isKeNhau[16][16][4];

void init(){    /// chuan bi truoc cac o co the ke nhau
    for(int i = 1;i < 16;i ++){
        for(int j = 1;j < 16;j ++){
            for(int k = 0;k < 4;k ++){
                if(isBitOn(i, k) && isBitOn(j, (k + 2) % 4)){
                    isKeNhau[i][j][k] = true;
                }   /// o thu nhat co gia tri i, o ke dang xet co gia tri j
            }       /// o thu nhat sang o thu hai theo huong k
        }
    }
}

int vis[10][10], ff;

int calScore(int a[10][10]){
    ff ++;

    int ans = 0;
    queue<pair<int, pair<int, int> > > q;
    /// previous direction, (x, y)
    /// not go to previous square

    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            if(! a[i][j] || (vis[i][j] == ff)) continue;

            q.push({-1, {i, j}}); vis[i][j] = ff;
            int d = 1; bool isCycle = 0;

            while(! q.empty() && !isCycle){
                int preDir = (q.front().first + 2) % 4;   /// dao nguoc lai
                int x = q.front().second.first, y = q.front().second.second;
                q.pop();

                for(int i = 0;i < 4;i ++){
                    if(i == preDir) continue;

                    int X = x + dx[i], Y = y + dy[i];

                    if(isInside(X, Y) && isKeNhau[a[x][y]][a[X][Y]][i]){
                        /// ke nhau
                        if(vis[X][Y] == ff){
                            isCycle = true;
                        } else {
                            d ++;   vis[X][Y] = ff;

                            q.push({i, {X, Y}});
                        }
                    }
                }
            }

         ///   cout << "??? " << i << " " << j << " " << isCycle << " " << d << "\n";

            if(!isCycle && (ans < d)){
                ans = d;
            }
        }
    }

    return ans;
}

const int M = int(1e9) + 7, csHash = 31;
unordered_set<int> uS;

bool isOk(){    /// kiem tra xem da di vao trang thai nay chua
    int cs = 1, maHash = 0;

    for(int i =  0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            cs = cs * csHash % M;

            maHash = (maHash + arr[i][j] * cs) % M;
        }
    }

    if(uS.find(maHash) == uS.end()) {
            uS.insert(maHash);

            return true;
    }

    return false;
}

time_t batDau, ketThuc;

int brr[10][10];

void local_search(){
    while(1){

        if(t.elapsed_time() > Titi){
            cout << best_t;
            exit(0);
        }

        for(int i = 0;i < 10;i ++){
            int index = rand() % int(best_t.size());

            for(int j = 0;j < 4;j ++){
                if(dir[j] == best_t[index]) continue;

                /// check hop le
                int x = bdX, y = bdY;
                bool ok = true;

                for(int k = 0;k < best_t.size();k ++){
                    x += dx[k], y += dy[k];

                    if(! isInside(x, y)){
                        ok = false;
                        break;
                    }
                }

                if(ok){
                    for(int i = 0;i < N;i ++){
                        for(int j = 0;j < N;j ++){
                            brr[i][j] = arr[i][j];
                        }
                    }

                    x = bdX, y = bdY;

                    for(int k = 0;k < best_t.size();k ++){
                        swap(brr[x][y], brr[x + dx[k]][y + dy[k]]);

                        x += dx[k], y += dy[k];
                    }

                    tmp_s = calScore(brr);

                    if(best_s < tmp_s){
                        best_s = tmp_s;
                        best_t[index] = dir[j];

                        for(int i = 0;i < N;i ++){
                            for(int j = 0;j < N;j ++){
                                arr[i][j] = brr[i][j];
                            }
                        }

                       /// goto next_stage;
                    }
                }
            }
        }

       /// next_stage:;
    }
}

void dfs(){
    tmp_s = calScore(arr);

    if((best_s == tmp_s) && (best_t.size() > tmp_t.size())){
        best_t = tmp_t;
    }

    if(best_s < tmp_s){
        best_s = tmp_s;
        best_t = tmp_t;
    }


    if(t.elapsed_time() > Ti){
        local_search();
    }

    if(H == T){ /// di het toi da so buoc



       /// cerr << "??? " << ++sl << "\n";
        return;
    }

    random_shuffle(idx, idx + 4);

    for(int i = 0;i < 4;i ++){  /// di tat ca cac huong co the
        int j = idx[i];

        int x = pos_x + dx[j], y = pos_y + dy[j];

        if(isInside(x, y)){
            H ++; tmp_t = tmp_t + "" + dir[j];

            arr[pos_x][pos_y] = arr[pos_x + dx[j]][pos_y + dy[j]];
            pos_x += dx[j]; pos_y += dy[j];
            arr[pos_x][pos_y] = 0;

            if(isOk()) dfs();

            H --; tmp_t.pop_back();

            arr[pos_x][pos_y] = arr[pos_x - dx[j]][pos_y - dy[j]];
            pos_x -= dx[j]; pos_y -= dy[j];
            arr[pos_x][pos_y] = 0;
        }
    }
}

signed main(){


    ios_base::sync_with_stdio(false); cin.tie(0);

    cin >> N >> T;

    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            char c; cin >> c;

            if((c >= '0') && (c <= '9')) arr[i][j] = int(c - '0');
            else arr[i][j] = int(c - 'a') + 10;

            if(! arr[i][j]){
                bdX = i; bdY = j;
                pos_x = i; pos_y = j;
            }
        }
    }
    srand(time(0));

    init();
    dfs();

    return 0;
}
