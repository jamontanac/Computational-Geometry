#include <iostream>
#include <iterator>
#include <vector>
#include <cstdlib>



struct combinations
{
    typedef std::vector<int> combination_t;

    // initialize status
   combinations(int N, int R) :
       completed(N < 1 || R > N),
       generated(0),
       N(N), R(R)
   {
       for (int c = 1; c <= R; ++c)
           curr.push_back(c);
   }

   // true while there are more solutions
   bool completed;

   // count how many generated
   int generated;

   // get current and compute next combination
   combination_t next()
   {
       combination_t ret = curr;

       // find what to increment
       completed = true;
       for (int i = R - 1; i >= 0; --i)
           if (curr[i] < N - R + i + 1)
           {
               int j = curr[i] + 1;
               while (i <= R-1)
                   curr[i++] = j++;
               completed = false;
               ++generated;
               break;
           }

       return ret;
   }

private:

   int N, R;
   combination_t curr;
};

int main(int argc, char **argv)
{
    int N = argc >= 2 ? std::atoi(argv[1]) : 5;
    int R = argc >= 3 ? std::atoi(argv[2]) : 2;
    combinations cs(N, R);
    while (!cs.completed)
    {
        combinations::combination_t c = cs.next();
        std::copy(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, ","));
        std::cout << std::endl;
    }
    return cs.generated;
}