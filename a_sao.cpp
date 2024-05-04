/**
    moi o se co cach tinh diem ve khoang cach voi vi tri ma no nen nam
    thi moi trang thai bang se co mot so diem nhat dinh
    di theo thuat toan a* de toi thieu hoa ham do xem sao

    kieu ke voi csnh duoi thi nen nam o phan tren cua bang -> xet khoang cach voi canh tren cung
    -> lay trung binh cong 4 khoang cach do
*/
#include <bits/stdc++.h>
using namespace std;

int arr[10][10], N, T; /// input
int H = 0, best_s = 0, tmp_s = 0; /// do sau hien tai, diem so tot nhat, diem so hien tai
string best_t = "", tmp_t = ""; /// loi giai tot nhat, loi giai hien tai

int dx[] = {0, -1, 0, 1}, dy[] = {-1, 0, 1, 0};
char dir[] = {'L', 'U', 'R', 'D'};
int idx[] = {0, 1, 2, 3};

int pos_x, pos_y;   /// position of empty square

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

int calScore(){
    ff ++;

    int ans = 0, diem = 0;
    queue<pair<int, pair<int, int> > > q;
    /// previous direction, (x, y)
    /// not go to previous square

    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            for(int k = 0;k < 4;k ++){
                if(isBitOn(arr[i][j], k)){
                    int kc = 0;

                    switch(k){
                        case 0 : {kc += N - j; break; }
                        case 1 : {kc += N - i; break; }
                        case 2 : {kc += j + 1; break; }
                        case 3 : {kc += i + 1; break; }
                    }

                    diem += (kc + 3) / 4;
                }
            }
            if(! arr[i][j] || (vis[i][j] == ff)) continue;

            q.push({-1, {i, j}}); vis[i][j] = ff;
            int d = 1; bool isCycle = 0;

            while(! q.empty() && !isCycle){
                int preDir = (q.front().first + 2) % 4;   /// dao nguoc lai
                int x = q.front().second.first, y = q.front().second.second;
                q.pop();

                for(int i = 0;i < 4;i ++){
                    if(i == preDir) continue;

                    int X = x + dx[i], Y = y + dy[i];

                    if(isInside(X, Y) && isKeNhau[arr[x][y]][arr[X][Y]][i]){
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

    return diem * ans;
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

void dfs(){
    tmp_s = calScore();

    if((best_s == tmp_s) && (best_t.size() > tmp_t.size())){
        best_t = tmp_t;
    }

    if(best_s < tmp_s){
        best_s = tmp_s;
        best_t = tmp_t;
    }

    time(&ketThuc);
    double tgian = double(ketThuc - batDau);
    if(tgian > 2.9){
        cout << best_t;
        exit(0);
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
    time(&batDau);

    ios_base::sync_with_stdio(false); cin.tie(0);

    cin >> N >> T;

    for(int i = 0;i < N;i ++){
        for(int j = 0;j < N;j ++){
            char c; cin >> c;

            if((c >= '0') && (c <= '9')) arr[i][j] = int(c - '0');
            else arr[i][j] = int(c - 'a') + 10;

            if(! arr[i][j]){
                pos_x = i; pos_y = j;
            }
        }
    }

    init();

  ///  cout << calScore(); return 0;

       /// chay het tgian thi thoi
    dfs();

    return 0;
}
