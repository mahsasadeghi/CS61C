#include <stdio.h>

typedef struct node
{
  int val;
  struct node* next;
} node;

/* FIXME: this function is buggy. */
int ll_equal(const node* a, const node* b)
{
  while (a != NULL && b != NULL)    // change: || --> &&  Reason is provided after main()
  {
    if (a->val != b->val)
      return 0;
    a = a->next;
    b = b->next;
  }
  /* lists are equal if a and b are both null */
  return a == b;
}

int main(int argc, char** argv)
{
  int i;
  node nodes[10];

  for (i=0; i<10; i++)
  {
    nodes[i].val = 0;
    nodes[i].next = NULL;
  }

  nodes[0].next = &nodes[1];
  nodes[1].next = &nodes[2];
  nodes[2].next = &nodes[3];

  printf("equal test 1 result = %d\n", ll_equal(&nodes[0], &nodes[0]));
  printf("equal test 2 result = %d\n", ll_equal(&nodes[0], &nodes[2]));

  return 0;
}

/* Reasoning:
 * Becuase even if one of a or b is not pointing to null, while's condition will
 * satisfy. Therefore, it might happen that one of them point to null. But by
 * changing || to && we can make sure that both of them are not pointing to null.
 * In this example, since we are passing the address of the 1st and 3rd node
 * (index: 0 and 2) to ll_equal, b will reach the end of the linked list before
 * a. So if we have ||, while will run because a is not pointing to null, but 
 * because b is pointing to null, it will cause a segmentation error.
 */