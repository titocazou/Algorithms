using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication1
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            MyClass myclass = new MyClass();
            string[] name1 = new string[4] { "cinema", "host", "aba", "train" };
            string[] name2 = new string[4] { "iceman", "shot", "bab", "rain" };
            myclass.check_anagrams(name1, name2);
        }
    }
}

class MyClass
{
    public void check_anagrams(string[] firstWords, string[] secondWords)
    {
        // Write your code here
        // To print results to the standard output you can use Console.WriteLine()
        // Example: Console.WriteLine("Hello world!");
        for (int i = 0; i < firstWords.Count(); ++i)
        {
            if (firstWords[i].Count() != secondWords[i].Count())
            {
                Console.WriteLine(0);
                continue;
            }
            char[] word1 = firstWords[i].ToCharArray();
            char[] word2 = secondWords[i].ToCharArray();

            Array.Sort<char>(word1);
            Array.Sort<char>(word2);

            bool fail = false;
            for (int j = 0; j < word1.Count(); ++j)
            {
                if (word1[j] != word2[j])
                {
                    fail = true;
                    break;
                }
            }
            if (fail)
            {
                Console.WriteLine(0);
            }
            else
            {
                Console.WriteLine(1);
            }
        }
        Console.Read();
    }
}

