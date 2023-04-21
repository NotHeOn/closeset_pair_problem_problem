#include <iostream>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<random>

using namespace std;

typedef struct {
    float x;
    float y;
}Point;

typedef struct {
    int index;
    float x;
    float y;
}A_Point;

//计算欧氏距离（的平方）
float dist(Point a, Point b) {
    float dx, dy;
    dx = a.x - b.x;
    dy = a.y - b.y;
    return(dx * dx + dy * dy);
}

//X方向上的merge
void merge_x(Point X[], int p, int q, int r)
{
    Point* bp = new Point[r - p + 1];
    int i, j, k;
    i = p; j = q + 1; k = 0;
    while (i <= q && j <= r) {
        if (X[i].x <= X[j].x) {
            bp[k++] = X[i++];
        }
        else {
            bp[k++] = X[j++];
        }
        if (i == q + 1) {
            for (; j <= r; j++) {
                bp[k++] = X[j];
            }
        }
        else {
            for (; i <= q; i++) {
                bp[k++] = X[i];
            }
        }
        k = 0;
        for (i = p; i <= r; i++) {
            X[i++] = bp[k++];
        }
        delete bp;
    }
}

//Y方向上的merge
void merge_y(A_Point Y[], int p, int q, int r)
{
    A_Point* bp = new A_Point[r - p + 1];
    int i, j, k;
    i = p; j = q + 1; k = 0;
    while (i <= q && j <= r) {
        if (Y[i].y <= Y[j].y) {
            bp[k++] = Y[i++];
        }
        else {
            bp[k++] = Y[j++];
        }
        if (i == q + 1) {
            for (; j <= r; j++) {
                bp[k++] = Y[j];
            }
        }
        else {
            for (; i <= q; i++) {
                bp[k++] = Y[i];
            }
        }
        k = 0;
        for (i = p; i <= r; i++) {
            Y[i++] = bp[k++];
        }
        delete bp;
    }
}

//X方向上的merge_sort
void merge_sort_x(Point X[], int len_X)
{
    int i, s, t = 1;
    while (t < len_X) {
        s = t; t = 2 * s; i = 0;
        while (i + t < len_X) {
            merge_x(X, i, i + s - 1, i + t - 1);
            i = i + t;
        }
        if (i + s < len_X) {
            merge_x(X, i, i + s - 1, len_X - 1);
        }
    }
}

//Y方向上的merge_sort
void merge_sort_y(A_Point Y[], int len_Y)
{
    int i, s, t = 1;
    while (t < len_Y) {
        s = t; t = 2 * s; i = 0;
        while (i + t < len_Y) {
            merge_y(Y, i, i + s - 1, i + t - 1);
            i = i + t;
        }
        if (i + s < len_Y) {
            merge_y(Y, i, i + s - 1, len_Y - 1);
        }
    }
}

//分治法主要实现函数
void closest(Point X[], A_Point Y[], int low, int high, Point& a, Point& b, float& d)
{
    if ((high - low) == 1) {
        a = X[low]; b = X[high]; d = dist(a, b);
    }
    else {
        if ((high - low) == 2) {
            float dl, dr;
            dl = dist(X[low], X[low + 1]);
            dr = dist(X[low], X[low + 2]);
            d = dist(X[low + 1], X[low + 2]);
            if ((dl <= dr) && (dl <= d)) {
                a = X[low]; b = X[low + 1]; d = dl;
            }
            else {
                if (dr <= d) {
                    a = X[low]; b = X[low + 2]; d = dr;
                }
                else {
                    a = X[low + 1]; b = X[low + 2];
                }
            }
        }
        else {
            int i, j, k, m;
            Point al, bl, ar, br;
            float dl, dr;
            m = (high + low) / 2 ;
            A_Point* SL = new A_Point[m + 1 - low];
            A_Point* SR = new A_Point[high - m];
            j = k = 0;
            for (i = 0; i <= high - low; i++) {
                if (Y[i].index <= m) {
                    SL[j++] = Y[i];
                }
                else {
                    SR[k++] = Y[i];
                }
            }
            closest(X, SL, low, m, al, bl, dl);
            closest(X, SR, m + 1, high, ar, br, dr);
            if (dl < dr) {
                a = al; b = bl; d = dl;
            }
            else {
                a = ar; b = br; d = dr;
            }
            A_Point* Z = new A_Point[high - low + 1];
            k = 0;
            for (i = 0; i <= high - low; i++) {
                float dx = Y[i].x - X[m].x;
                if ((fabs(dx) < d)) {
                    if (dx <= 0) {
                        Z[k].index = 0;
                    }
                    else {
                        Z[k].index = 1;
                    }
                    Z[k].x = Y[i].x;
                    Z[k++].x = Y[i].y;
                }
            }
            for (i = 0; i < k; i++) {
                for (j = i + 1; (j < k) && (Z[i].index != Z[j].index) && (Z[j].y - Z[i].y < d); j++) {
                    Point tmp1{ Z[i].x,Z[i].y }, tmp2{ Z[j].x,Z[j].y };
                    dl = dist(tmp1, tmp2);
                    if (dl < d) {
                        a = tmp1; b = tmp2; d = dl;
                    }
                }
            }
            delete SL; delete SR; delete Z;
        }
    }
}

//分治法启动函数
void closest_pair(Point X[], int n, Point& a, Point& b, float& d)
{
    if (n < 2) {
        d = 0;
    }
    else {
        merge_sort_x(X, n);
        A_Point* Y = new A_Point[n];
        for (int i = 0; i < n; i++) {
            Y[i].index = i;
            Y[i].x = X[i].x;
            Y[i].y = X[i].y;
        }
        merge_sort_y(Y, n);
        closest(X, Y, 0, n - 1, a, b, d);
        d = sqrt(d);
        delete Y;
    }
}

//穷举法函数
void cp_brute_force(Point X[], int n,Point& a,Point& b,float& d) 
{
    int i, j;
    a = X[0]; b = X[1];
    d = dist(a, b);
    float md;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            md = dist(X[i], X[j]);
            if (md < d) {
                a = X[i]; b = X[j]; d = md;
            }
        }
    }
}

//点集生成函数
void pointset_generator(Point X[], int n,float lower_bound,float upper_bound)
{
    default_random_engine e(time(0));
    uniform_real_distribution<float> u(lower_bound, upper_bound);
    for (int i = 0; i < n; i++) {
        X[i].x = u(e);
        X[i].y = u(e);
    }
}


int main()
{
    int n = 10;
    Point* X = new Point[10];
    float lower_bound = -20.0;
    float upper_bound = 20.0;
    pointset_generator(X, n, lower_bound, upper_bound);
    Point bf_a, bf_b, a, b;
    float bf_d, d;
    cp_brute_force(X, n, bf_a, bf_b, bf_d);
    closest_pair(X, n, a, b, d);
    cout << "测试集大小:" << n << "(点)" << endl;
    cout << "穷举法结果:" << endl;
    cout << "最接近点对:(" << bf_a.x << "," << bf_a.y << ")、(" << bf_b.x << "," << bf_b.y << ")" << ", 距离:" << bf_d << endl;
    cout << "分治法结果:" << endl;
    cout << "最接近点对:(" << a.x << "," << a.y << ")、(" << b.x << "," << b.y << ")" << ", 距离:" << d << endl;
}
