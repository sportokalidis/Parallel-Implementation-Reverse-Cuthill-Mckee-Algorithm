// C++ program for Implementation of
// Reverse Cuthill Mckee Algorithm

#include <bits/stdc++.h>
using namespace std;

vector<double> globalDegree;

int findIndex(vector<pair<int, double> > a, int x)
{
    for (int i = 0; i < a.size(); i++)
        if (a[i].first == x)
            return i;
    return -1;
}

bool compareDegree(int i, int j)
{
    return ::globalDegree[i] - ::globalDegree[j];
}

template <typename T>
ostream& operator<<(ostream& out, vector<T> const& v)
{
    for (int i = 0; i < v.size(); i++)
        out << v[i] << ' ';
    return out;
}

class ReorderingSSM {
private:
    vector<vector<double> > _matrix;

public:
    // Constructor and Destructor
    ReorderingSSM(vector<vector<double> > m)
    {
        _matrix = m;
    }

    ReorderingSSM() {}
    ~ReorderingSSM() {}

    // class methods

    // Function to generate degree of all the nodes
    vector<double> degreeGenerator()
    {

        vector<double> degrees;

        for (int i = 0; i < _matrix.size(); i++) {
            double count = 0;
            for (int j = 0; j < _matrix[0].size(); j++) {
                count += _matrix[i][j];
            }

            degrees.push_back(count);
        }

        return degrees;
    }

    // Implementation of Cuthill-Mckee algorithm
    vector<int> CuthillMckee()
    {
        vector<double> degrees = degreeGenerator();  // the sum of matrix's rows

        ::globalDegree = degrees;

        queue<int> Q;
        vector<int> R;
        vector<pair<int, double> > notVisited;

        for (int i = 0; i < degrees.size(); i++)
            notVisited.push_back(make_pair(i, degrees[i]));

        // Vector notVisited helps in running BFS
        // even when there are dijoind graphs
        while (notVisited.size()) {

            int minNodeIndex = 0;

            // find the possition of minimum degree
            for (int i = 0; i < notVisited.size(); i++)
                if (notVisited[i].second < notVisited[minNodeIndex].second)
                    minNodeIndex = i;

            // add the possition of minimum degree in Q
            Q.push(notVisited[minNodeIndex].first);

            // delete this node from no visited
            notVisited.erase(notVisited.begin()
                                    + findIndex(notVisited,
                                                notVisited[Q.front()].first));

            // Simple BFS
            while (!Q.empty()) {

                vector<int> toSort;

                for (int i = 0; i < _matrix[0].size(); i++) {
                    // find all neighbors of node that is notVisited and add neighbors to toSort
                    if (i != Q.front() && _matrix[Q.front()][i] == 1
                        && findIndex(notVisited, i) != -1) {
                        toSort.push_back(i);
                        notVisited.erase(notVisited.begin()
                                           + findIndex(notVisited, i));
                    }
                }

                // sort the toSort vector
                sort(toSort.begin(), toSort.end(), compareDegree);

                // add neighbors to Q (the minimum degree first)
                for (int i = 0; i < toSort.size(); i++)
                    Q.push(toSort[i]);

                // Add the current node in R and delete it from Q
                R.push_back(Q.front());
                Q.pop();
            }
        }

        return R;
    }

    // Implementation of reverse Cuthill-Mckee algorithm
    vector<int> ReverseCuthillMckee()
    {

        vector<int> cuthill = CuthillMckee();

        int n = cuthill.size();

        if (n % 2 == 0)
            n -= 1;

        n = n / 2;

        for (int i = 0; i <= n; i++) {
            int j = cuthill[cuthill.size() - 1 - i];
            cuthill[cuthill.size() - 1 - i] = cuthill[i];
            cuthill[i] = j;
        }

        return cuthill;
    }
};

// Driver Code
int main()
{
    int num_rows = 10;

    vector<vector<double> > matrix;

    for (int i = 0; i < num_rows; i++) {
        vector<double> datai;

        for (int j = 0; j < num_rows; j++)
            datai.push_back(0.0);

        matrix.push_back(datai);
    }

    // This is the test graph,
    // check out the above graph photo
    matrix[0] = { 0, 1, 0, 0, 0, 0, 1, 0, 1, 0 };
    matrix[1] = { 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 };
    matrix[2] = { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 };
    matrix[3] = { 0, 0, 0, 0, 1, 1, 0, 0, 1, 0 };
    matrix[4] = { 0, 1, 1, 1, 0, 1, 0, 0, 0, 1 };
    matrix[5] = { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 };
    matrix[6] = { 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
    matrix[7] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 };
    matrix[8] = { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 };
    matrix[9] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 };

    ReorderingSSM m(matrix);

    vector<int> r = m.ReverseCuthillMckee();

    cout << "Permutation order of objects: " << r << endl;

    return 0;
}
