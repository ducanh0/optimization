/**
    luu lai k trang thai tot nhat hien tai, phat trien cac trang thai ke
    -> loc ra roi giu lai k trang thai tot nhat
    -> lap t lan

    bfs

    idea kha giong GA, kieu di xuong tung buoc 1 , giu lay k trang thai tot nhat
    tung tang , r di xuogn dan dan , moi tang chi giu lai k trang thai tot nhat
*/
#include <bits/stdc++.h>
using namespace std;

using namespace chrono;

class timer: high_resolution_clock {
    const time_point start_time;
public:
    timer(): start_time(now()) {}
    rep elapsed_time() const { return duration_cast<milliseconds>(now()-start_time).count(); } };

const int Ti = 2900;
timer t; /// create a timer object

const int K = 50;

int arr[10][10], brr[10][10], N, T; /// input
vector<pair<string, pair<int, int> > > V;
int bdX, bdY;

int best_s;
string best_t = "";

char dir[] = {'L', 'U', 'R', 'D'};
int dx[] = {0, -1, 0, 1};
int dy[] = {-1, 0, 1, 0};
int idx[] = {0, 1, 2, 3};

bool isKeNhau[16][16][4];

bool isBitOn(int i,int j){  /// whether bit j of i equal 1 ?
    return ((i >> j) & 1);
}

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

bool isInside(int x,int y){
    return 1 - (x < 0 || x >= N || y < 0 || y >= N);
}

int vis[10][10], ff;

int calScore(string nst){
    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            brr[i][j] = arr[i][j];
        }
    }

    int x = bdX, y = bdY;

    for(char i : nst){
        int huong;

        switch(i){
            case 'L' : {
                huong = 0;
                break;
            }
            case 'U' : {
                huong = 1;
                break;
            }
            case 'R' : {
                huong = 2;
                break;
            }
            case 'D' : {
                huong = 3;
                break;
            }
            default:{
                huong = 4;
                break;
            }
        }

        swap(brr[x][y], brr[x + dx[huong]][y + dy[huong]]);

        x += dx[huong], y += dy[huong];
    }

    ff ++;

    int ans = 0;
    queue<pair<int, pair<int, int> > > q;
    /// previous direction, (x, y)
    /// not go to previous square

    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            if(! brr[i][j] || (vis[i][j] == ff)) continue;

            q.push({-1, {i, j}}); vis[i][j] = ff;
            int d = 1; bool isCycle = 0;

            while(! q.empty() && !isCycle){
                int preDir = (q.front().first + 2) % 4;   /// dao nguoc lai
                int x = q.front().second.first, y = q.front().second.second;
                q.pop();

                for(int i = 0;i < 4;i ++){
                    if(i == preDir) continue;

                    int X = x + dx[i], Y = y + dy[i];

                    if(isInside(X, Y) && isKeNhau[brr[x][y]][brr[X][Y]][i]){
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
            }
        }
    }

    init();

    V.push_back({"", {bdX, bdY}});
    best_s = calScore("");

    /// bat dau qua trinh
    /// chay den het tgian thi thoi
     while ((t.elapsed_time() < Ti) &&(T -- > 0) ) {



        /// 15 thang tot nhat the he moi + 5 thang tot nhat the he cu
        vector<pair<string, pair<int, int> > > v;

        for(auto s : V){
            for(int i = 0;i < 4;i ++){
                    int x = s.second.first + dx[i],
                        y = s.second.second + dy[i];

                    if(isInside(x, y)){
                            string newS = s.first + "" + dir[i];

                            v.push_back({newS, {x, y}});
                    }
            }
        }

      ///  for(auto i : v) cout << i.first << "\n";
       /// v.clear();
        sort(v.begin(), v.end(), [&](const pair<string, pair<int, int> > &x1,
                                     pair<string, pair<int, int> > & x2 ){
               return calScore(x1.first) > calScore(x2.first);
        });

        if(! v.empty()){
             int diem = calScore(v[0].first);

            if(diem > best_s){
                best_s = diem;
                best_t = v[0].first;
            }
        }

        for(int i = 0;i < K;i ++){
            if(int(v.size()) <= i) break;

            if(int(V.size()) <= i) {
                V.push_back(v[i]);
            } else {
                V[i] = v[i];
            }
        }
    }


    cout << best_t;

    return 0;
}

