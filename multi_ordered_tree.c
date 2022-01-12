//
// AED, January 2022
//
// Solution of the second practical assignement (multi-ordered tree)
//
// Place your student numbers and names here
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AED_2021_A02.h"

//
// the custom tree node structure
//
// we want to maintain three ordered trees (using the same nodes!), so we need three left and three right pointers
// so, when inserting a new node we need to do it three times (one for each index), so we will end upo with 3 three roots
//

typedef struct tree_node_s
{
  char name[MAX_NAME_SIZE + 1];                         // index 0 data item
  char zip_code[MAX_ZIP_CODE_SIZE + 1];                 // index 1 data item
  char telephone_number[MAX_TELEPHONE_NUMBER_SIZE + 1]; // index 2 data item
  struct tree_node_s *left[3];                          // left pointers (one for each index) ---- left means smaller
  struct tree_node_s *right[3];                         // right pointers (one for each index) --- right means larger
}
tree_node_t;

//
// the node comparison function (do not change this)
//

int compare_tree_nodes(tree_node_t *node1,tree_node_t *node2,int main_idx)
{
  int i,c;

  for(i = 0;i < 3;i++)
  {
    if(main_idx == 0)
      c = strcmp(node1->name,node2->name);
    else if(main_idx == 1)
      c = strcmp(node1->zip_code,node2->zip_code);
    else
      c = strcmp(node1->telephone_number,node2->telephone_number);
    if(c != 0)
      return c; // different on this index, so return
    main_idx = (main_idx == 2) ? 0 : main_idx + 1; // advance to the next index
  }
  return 0;
}

//
// tree insertion routine (place your code here)
//

void tree_insert(tree_node_t **roots, tree_node_t *node, int main_index)
{
    /* If the tree is empty, return a new Node */
    if (roots[main_index] == NULL)
    {
        roots[main_index] = node;
        return;
    }
    /* Otherwise, recur down the tree */
    if (compare_tree_nodes(roots[main_index], node, main_index) > 0)
        tree_insert(roots[main_index]->left, node, main_index);
    else if (compare_tree_nodes(roots[main_index], node, main_index) < 0)
        tree_insert(roots[main_index]->right, node, main_index);
    else
    {
        fprintf(stderr,"ERROR generating tree, two equal people!\n");
        fprintf(stderr,"Please chose another student number.\n");
        exit(1);
    }
}

//
// tree search routine (place your code here)
//

tree_node_t *find(tree_node_t *root, tree_node_t node, int main_index)
{
    if (root == NULL)
        return NULL;

    if (compare_tree_nodes(root, &node, main_index) > 0)
        return find(root->left[main_index], node, main_index);
    else if (compare_tree_nodes(root, &node, main_index) < 0)
        return find(root->right[main_index], node, main_index);
    else
        return root;
}

//
// tree depdth
//

int tree_depth(tree_node_t *root, int main_index)
{
    if (root == NULL)
        return 0;

    int l = tree_depth(root->left[main_index], main_index);
    int r = tree_depth(root->right[main_index], main_index);

    if (l > r)
        return l + 1;

    return r + 1;
}

//
// list, i,e, traverse the tree (place your code here)
//

void list(tree_node_t *root, int main_index)
{
    if (root != NULL)
    {
        list(root->left[main_index], main_index);
        printf("Person #\n");
        printf("    name --------------- %s\n", root->name);
        printf("    zip code ----------- %s\n", root->zip_code);
        printf("    telephone number --- %s\n", root->telephone_number);
        list(root->right[main_index], main_index);
    }
}

//
// main program
//

int main(int argc,char **argv)
{
  double dt;

  // process the command line arguments
  if(argc < 3)
  {
    fprintf(stderr,"Usage: %s student_number number_of_persons [options ...]\n",argv[0]);
    fprintf(stderr,"Recognized options:\n");
    fprintf(stderr,"  -list[N]              # list the tree contents, sorted by key index N (the default is index 0)\n");
    // place a description of your own options here
    return 1;
  }
  int student_number = atoi(argv[1]);
  if(student_number < 1 || student_number >= 1000000)
  {
    fprintf(stderr,"Bad student number (%d) --- must be an integer belonging to [1,1000000[\n",student_number);
    return 1;
  }
  int n_persons = atoi(argv[2]);
  if(n_persons < 3 || n_persons > 10000000)
  {
    fprintf(stderr,"Bad number of persons (%d) --- must be an integer belonging to [3,10000000]\n",n_persons);
    return 1;
  }
  // generate all data
  tree_node_t *persons = (tree_node_t *)calloc((size_t)n_persons,sizeof(tree_node_t));
  if(persons == NULL)
  {
    fprintf(stderr,"Output memory!\n");
    return 1;
  }
  aed_srandom(student_number);
  for(int i = 0;i < n_persons;i++)
  {
    random_name(&(persons[i].name[0]));
    random_zip_code(&(persons[i].zip_code[0]));
    random_telephone_number(&(persons[i].telephone_number[0]));
    for(int j = 0;j < 3;j++)
      persons[i].left[j] = persons[i].right[j] = NULL; // make sure the pointers are initially NULL
  }
  // create the ordered binary trees
  dt = cpu_time();
  tree_node_t *roots[3]; // three indices, three roots
  for(int main_index = 0;main_index < 3;main_index++)
    roots[main_index] = NULL;
  for(int i = 0;i < n_persons;i++)
    for(int main_index = 0;main_index < 3;main_index++)
        tree_insert(roots, &persons[i], main_index); // place your code here to insert &(persons[i]) in the tree with number main_index
  dt = cpu_time() - dt;
  printf("Tree creation time (%d persons): %.3es\n",n_persons,dt);
  // search the tree
  for(int main_index = 0;main_index < 3;main_index++)
  {
    dt = cpu_time();
    for(int i = 0;i < n_persons;i++)
    {
      tree_node_t n = persons[i]; // make a copy of the node data
      if(find(roots[main_index], n, main_index) != &(persons[i])) // place your code here to find a given person, searching for it using the tree with number main_index
      {
        fprintf(stderr,"person %d not found using index %d\n",i,main_index);
        return 1;
      }
    }
    dt = cpu_time() - dt;
    printf("Tree search time (%d persons, index %d): %.3es\n",n_persons,main_index,dt);
  }
  // compute the largest tree depdth
  for(int main_index = 0;main_index < 3;main_index++)
  {
    dt = cpu_time();
    int depth = tree_depth(roots[main_index], main_index); // place your code here to compute the depth of the tree with number main_index
    dt = cpu_time() - dt;
    printf("Tree depth for index %d: %d (done in %.3es)\n",main_index,depth,dt);
  }
  // process the command line optional arguments
  for(int i = 3;i < argc;i++)
  {
    if(strncmp(argv[i],"-list",5) == 0)
    { // list all (optional)
      int main_index = atoi(&(argv[i][5]));
      if(main_index < 0)
        main_index = 0;
      if(main_index > 2)
        main_index = 2;
      printf("List of persons:\n");
      list(roots[main_index], main_index); // place your code here to traverse, in order, the tree with number main_index
    }
    // place your own options here
  }
  // clean up --- don't forget to test your program with valgrind, we don't want any memory leaks
  free(persons);
  return 0;
}
