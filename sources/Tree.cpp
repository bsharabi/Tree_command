#include "Tree.hpp"

using namespace ariel;
namespace ariel
{

    Tree::Tree() : root(nullptr), dirNumber(0), fileNumber(0) {}
    Tree::~Tree() {}

    Tree &Tree::add(const string &name, int level, const string &permitions, const string &user, const string &group, const string &size, const string &parentName)
    {
        if (name.empty())
        {
            throw invalid_argument("empty string name");
        }
        if (root == nullptr)
        {
            root = new Node(name, level, permitions, user, group, size);
            return *this;
        }
        if (parentName.empty())
        {
            throw invalid_argument("empty string");
        }
        if (!permitions.empty())
        {
            fileNumber += (permitions[0] != 'd') ? 1 : 0;
            dirNumber += (permitions[0] != 'd') ? 0 : 1;
        }
        add_node(root, parentName, name, level, permitions, user, group, size);
        return *this;
    }

    void Tree::add_node(Node *root, const string &parentName, const string &name, int level, const string &permitions, const string &user, const string &group, const string &size)
    {

        if (root->_name == parentName && root->_level == level - 1)
        {
            Node *node = new Node(name, level, permitions, user, group, size);
            root->_children.push_back(node);
            std::sort(root->_children.begin(), root->_children.end(), compare);

            return;
        }

        int l = root->_children.size();
        for (unsigned int i = 0; i < l; i++)
        {
            add_node(root->_children[i], parentName, name, level, permitions, user, group, size);
        }
    }

    std::ostream &operator<<(ostream &out, const Tree &p1)
    {
        p1.root->printTree(out,p1.dirNumber,p1.fileNumber);
        return out;
    }

}