#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <map>
#include <list>
#include <regex>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

namespace ariel
{

    /**
     * Tree Public class
     *
     * This class represents a node in the tree
     */
    class Tree
    {

    private:
        /**
         * Node Private subclass
         *
         * This class is used to store a single node in the tree,
         * and it also stores the file name, type, level, size, group, permissions, user and
         * its children in the tree if they exist
         */
        class Node
        {
        public:
            string _name;
            int _level;
            string _permitions;
            string _user;
            string _group;
            string _size;
            vector<Node *> _children;
            // ------------------------------ Constructor --------------------------------------
            /**
             * Constructor.
             * @param name file's name
             * @param level node's level in the tree
             * @param user user's name
             * @param group group's name
             * @param size  file's size
             * @param permitions Linux file permissions
             * @return None.
             */
            Node(const string &name, int level, const string &permitions, const string &user, const string &group, const string &size)
                : _name(name), _level(level), _permitions(permitions), _size(size), _group(group), _user(user){};

        private:
            void printSubtree(const std::string &prefix, std::ostream &out)
            {
                if (_children.empty())
                {
                    return;
                }
                out << prefix;
                size_t n_children = _children.size();
                out << "\033[1;31m" << (n_children > 1 ? "├── " : "") << "\033[0m";

                unsigned long maxSize = 0;
                unsigned long PrefixSize;

                for (size_t i = 0; i < n_children; ++i)
                {
                    if (maxSize < _children[i]->_size.length())
                    {
                        maxSize = _children[i]->_size.length();
                    }
                }

                for (size_t i = 0; i < n_children; ++i)
                {
                    Node *c = _children[i];
                    bool isDir = c->_permitions[0] == 'd';
                    if (i < n_children - 1)
                    {
                        PrefixSize = maxSize - c->_size.length();

                        if (i > 0)
                        {
                            out << prefix << "\033[1;31m"
                                << "├── "
                                << "\033[0m";
                        }
                        bool printStrand = n_children > 1 && !c->_children.empty();
                        string newPrefix = prefix + (printStrand ? "\033[1;31m│\033[0m   " : "   ");
                        out << "\033[1;33m"
                            << "[" << c->_permitions << " " << c->_user << "\t " << c->_group << "\t\t";
                        for (int space = 0; space < PrefixSize; space++)
                            out << " ";
                        out << c->_size << "]  "
                            << ((isDir) ? "\u001B[42m" : "") << c->_name << "\033[0m"
                            << "\n";
                        c->printSubtree(newPrefix, out);
                    }
                    else
                    {
                        out << (n_children > 1 ? prefix : " ") << "\033[1;31m"
                            << "└── "
                            << "\033[0m";
                        out << "\033[1;33m"
                            << "[" << c->_permitions << " " << c->_user << "\t " << c->_group << "\t\t";
                        for (int space = 0; space < PrefixSize; space++)
                            out << " ";
                        out << c->_size << "]  "
                            << ((isDir) ? "\u001B[42m" : "") << c->_name << "\033[0m"
                            << "\n";
                        c->printSubtree(prefix + " ", out);
                    }
                }
            }

        public:
            /**
             * print tree
             *
             * @param out
             * @return None.
             */
            void printTree(std::ostream &out, int dirN, int fileN)
            {
                out << "\033[0;32m"

                    << _name << "\033[0m"
                    << "\n";
                printSubtree("", out);
                out << "\033[0m" << endl
                    << dirN << " directories, " << fileN << " files";
            }
        };

        // ---------------------------- class Tree variables --------------------------
        Node *root;
        int fileNumber;
        int dirNumber;

    public:
        // ----------------------------------- Constuctors --------------------------------
        /**
         * Constructor.
         * @return None.
         */
        Tree();
        // ----------------------------------- Destructor --------------------------------
        /**
         * A  destructor is a member function that is invoked automatically when the object goes out of scope or is explicitly destroyed by a call to delete
         * Destructor.
         * @return None.
         */
        ~Tree();
        // ----------------------------------- Copy Constructor --------------------------------
        /**
         * Copy Constructor
         * A copy constructor is called when an object is passed by value..
         * @return None.
         */
        Tree(Tree &) = default;
        /**
         * Copy Constructor
         * A copy constructor is called when an object is passed by value..
         * @return None.
         */
        Tree(Tree &&) = default;
        // ----------------------------------- functions ----------------------------------

        /**
         * A function that allows adding node to the tree,
         * @param name file's name
         * @param level node's level in the tree
         * @param parentName parent file's name
         * @param user user's name
         * @param group group's name
         * @param size  file's size
         * @param permitions Linux file permissions
         * @return Tree.
         */
        Tree &add(const string &, int, const string &, const string &, const string &, const string &, const string &);

        static bool(compare)(const ariel::Tree::Node *obj1, const ariel::Tree::Node *obj2)
        {
            return obj1->_name < obj2->_name;
        }
        // ---------------------------------- Overloading ----------------------------------
        /**
         * operator * overloading.
         *
         * @param out
         * @param Tree The Object
         * @return out.
         */
        friend std::ostream &operator<<(std::ostream &, const Tree &);

    private:
        /**
         * A function that allows new node to be brought into the tree
         * @param Node  The root of the tree
         * @param parentName name of Manager
         * @param name file's name
         * @param level node's level in the tree
         * @param permitions Linux file permissions
         * @param user user's name
         * @param group group's name
         * @param size  file's size
        
         * @return None.
         */
        void
        add_node(Node *, const string &, const string &, int, const string &, const string &, const string &, const string &);
    };
}