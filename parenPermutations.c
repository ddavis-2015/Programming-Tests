/*
 * Copyright (c) 2019 David Davis.  All rights reserved.
 *
 * compiled under MacOSX using:
 * 		cc -Wall -ansi -pedantic crypticProgrammerTest.c -o crypticProgrammerTest
 */

static const char copyright[] __attribute((used)) =
	"Copyright (c) 2019 David Davis.  All rights reserved";


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef enum
{
        INVALID,
        ADD,
        SUBTRACT,
        MULTIPLY
} NodeType;

struct _Node;

typedef struct _Node
{
        struct _Node *  next;
        struct _Node *  prev;
        NodeType        type;
        int             lhs, rhs, value;
        int             leftParens, rightParens;
        int             used;
        int             computed;
} Node;

struct _Value;

typedef struct _Value
{
        struct _Value * next;
        int             value;
} Value;

struct _ParenNode;

typedef struct _ParenNode
{
        struct _ParenNode *     prev;
        struct _ParenNode *     subList[2];

        struct
        {
                int     left;
                int     right;
        } Parens[1];
} ParenNode;


static Node * newNode(void)
{
        Node * np = (Node *)calloc(1, sizeof(*np));

        if (np == NULL)
                return NULL;

        np->type = INVALID;

		return np;
}

static Node * parseExpr(const char * s)
{
        Node *  current;
        Node *  prev;
        Node *  top;

        current = newNode();
        if (current == NULL)
                return NULL;
        top = prev = current;
        
        while (*s != '\0')
        {
                if (isspace(*s))
                {
                        s++;
                        continue;
                }

                if (isdigit(*s))
                {
                        if (current->type == INVALID)
                        {
                                current->lhs = atoi(s);
                                while (isdigit(*s))
                                        s++;

                                continue;       /* back to top of while loop */
                        }
                        else
                        {
                                /* node is complete; set all remaining a fields */
                                current->rhs = atoi(s);

                                if (current->type == ADD)
                                        current->value = current->lhs + current->rhs;
                                else if (current->type == SUBTRACT)
                                        current->value = current->lhs - current->rhs;
                                else
                                        current->value = current->lhs * current->rhs;

                                /* add node to the list */
                                if (current != top)
                                {
                                        current->prev = prev;
                                        prev->next = current;
                                }
                                prev = current;
                                current = newNode();
                                if (current == NULL)
                                        return top;

                                continue;       /* back to top of while loop */
                        }
                }

                if (*s == '+')
                        current->type = ADD;
                else if (*s == '-')
                        current->type = SUBTRACT;
                else if (*s == '*')
                        current->type = MULTIPLY;

                s++;
        }

        /* free the unattached node we were working on */
        if (current->type == INVALID && top != current)
                free(current);

        return top;
}

static int nodeCount(const Node * np)
{
        int     count = 1;
        
        while (np->next != NULL)
        {
                count++;

                np = np->next;
        }

        return count;
}

static void freeNodes(Node * np)
{
        Node *  current;

        current = np;
        while (current != NULL)
        {
                np = np->next;
                free(current);
                current = np;
        }
}

static Value * insertValue(int value, Value * list)
{
        Value * current;

        current = (Value *)malloc(sizeof(*current));
        if (current == NULL)
                return NULL;

        current->next = NULL;
        current->value = value;

        if (list != NULL)
                list->next = current;

        return current;
}

static void freeValueList(Value * vp)
{
        Value * current;

        current = vp;
        while (current != NULL)
        {
                vp = vp->next;
                free(current);
                current = vp;
        }
}

static int findValue(int value, Value * list)
{
        while (list != NULL)
        {
                if (value == list->value)
                        return 1;
                list = list->next;
        }

        return 0;
}

static void printValueList(Value * list)
{
        int     unique = 0;
        Value * vp;

        vp = list;
        while (vp != NULL)
        {
                unique++;

                vp = vp->next;
        }

        printf("%d unique { ", unique);

        vp = list;
        while (vp != NULL)
        {
                printf("%d ", vp->value);

                vp = vp->next;
        }

        printf("}\n");
}

static int computeRHS(Node * np, int lhs)
{
        Node *  current = np;
        int     rhs;
        int     depth;
        int     i;

        depth = np->leftParens - np->rightParens;

        if (depth < 0)
        {
                current->computed = 1;

                if (current->leftParens && current->rightParens)
                        return current->value;

                rhs = current->rhs;

                if (current->type == ADD)
                        lhs += rhs;
                else if (current->type == SUBTRACT)
                        lhs -= rhs;
                else
                        lhs *= rhs;
        }
        else if (depth == 0)
        {
                current->computed = 1;

                if (current->leftParens && current->rightParens)
                        return current->value;

                rhs = computeRHS(current->next, lhs);

                if (current->type == ADD)
                        lhs += rhs;
                else if (current->type == SUBTRACT)
                        lhs -= rhs;
                else
                        lhs *= rhs;
        }
        else
        {
                if (current->rightParens && current->leftParens)
                        lhs = current->value;
                else if (current->leftParens)
                        lhs = current->lhs;

                for (i = 0; i < depth; i++)
                {
                        rhs = computeRHS(current->next, lhs);

                        if (current->rightParens == 0)
                        {
                                if (current->type == ADD)
                                        lhs += rhs;
                                else if (current->type == SUBTRACT)
                                        lhs -= rhs;
                                else
                                        lhs *= rhs;
                        }
                        else
                        {
                                lhs = rhs;
                        }

                        current->computed = 1;
                        while (current->next != NULL && current->next->computed)
                                current = current->next;
                }
        }

        return lhs;
}

static int computeExpr(Node * top)
{
        Node *  np;

        for (np = top; np != NULL; np = np->next)
                np->computed = 0;

        return computeRHS(top, 0);
}

static ParenNode * newParenNode(ParenNode * origNode, int maxParens)
{
        ParenNode *     newNode;

        newNode = calloc(1, sizeof(*newNode) + ((maxParens - 1) * sizeof(newNode->Parens[0])));
        if (newNode == NULL)
                return NULL;

        if (origNode != NULL)
                memcpy(newNode->Parens, origNode->Parens, maxParens * sizeof(newNode->Parens[0]));

        return newNode;
}

static void assignRightParens(ParenNode * pnp, int maxParens)
{
        int *   stack;
        int     i;
        int     stackIndex;

        assert((stack = calloc(maxParens, sizeof(*stack))) != NULL);

        for (i = 0, stackIndex = -1; i < maxParens; i++)
        {
                int     rightParens = 0;

                /* push left parenthesis count onto stack */
                if (pnp->Parens[i].left)
                        stack[++stackIndex] = pnp->Parens[i].left;

                /* check if we cannot place a right parenthesis at this level */
                if (i + 1 < maxParens && pnp->Parens[i + 1].left)
                {
                        pnp->Parens[i].right = 0;
                        continue;
                }

                while (stackIndex >= 0)
                {
                        rightParens++;
                        if (--stack[stackIndex])
                                break;

                        /* pop the stack and continue */
                        stackIndex--;
                }

                pnp->Parens[i].right = rightParens;
        }

        free(stack);
}

static void buildParenList(ParenNode * pnp, int index, int maxParens)
{
        ParenNode *     newNode;

        if (index == (maxParens - 1))
                return;

        /* dont generate an illegal left parenthesis */
        if (index == 0 && pnp->Parens[index].left == 1)
                return;

        /* see if we can build a subtree */
        if (pnp->Parens[index].left > 0)
        {
                /* dont generate an illegal left parenthesis */
                if ((pnp->Parens[index + 1].left + 1) > (maxParens - (index + 1)))
                        return;

                newNode = newParenNode(pnp, maxParens);
                if (newNode == NULL)
                        return;

                newNode->Parens[index].left--;
                newNode->Parens[index + 1].left++;
                assignRightParens(newNode, maxParens);

                newNode->prev = pnp;
                if (pnp->subList[0] == NULL)
                        pnp->subList[0] = newNode;
                else
                        pnp->subList[1] = newNode;

                if (newNode->Parens[index].left > 0)
                        buildParenList(newNode, index, maxParens);

                if (newNode->Parens[index + 1].left > 0)
                        buildParenList(newNode, index + 1, maxParens);
        }
}

static ParenNode * nextParenNode(ParenNode * origNode)
{
        ParenNode *     newNode = origNode;

        for (;;)
        {
                if (newNode->subList[0] != NULL)
                {
                        newNode = newNode->subList[0];
                        continue;
                }
                if (newNode->subList[1] != NULL)
                {
                        newNode = newNode->subList[1];
                        continue;
                }

                if (newNode == origNode)
                {
                        newNode = newNode->prev;

                        if (newNode == NULL)
                        {
                                free(origNode);
                                return NULL;
                        }

                        free(origNode);
                        if (newNode->subList[0] == origNode)
                                newNode->subList[0] = NULL;
                        else
                                newNode->subList[1] = NULL;

                        if (newNode->subList[0] != NULL || newNode->subList[1] != NULL)
                                continue;
                }

                break;
        }

        return newNode;
}

void parenPermutations(char * expression)
{
        Node *          exprNodes;
        Value *         valueList = NULL;
        Value *         valueListTop = NULL;
        ParenNode *     parenListTop;
        ParenNode *     pnp;
        int             maxParens;

        exprNodes = parseExpr(expression);
        if (exprNodes == NULL)
                return;

        /* trivial expressions */
        if (exprNodes->type == INVALID)
        {
                printf("1 unique { %d }\n", exprNodes->lhs);
                freeNodes(exprNodes);
                
                return;
        }
        if (exprNodes->next == NULL)
        {
                printf("1 unique { %d }\n", exprNodes->value);
                freeNodes(exprNodes);

                return;
        }

        /*
         * okay we have at least 2 operators, compute all of the permutations
         */
        maxParens = nodeCount(exprNodes);
        parenListTop = newParenNode(NULL, maxParens);
        if (parenListTop == NULL)
        {
                freeNodes(exprNodes);

                return;
        }
        parenListTop->Parens[0].left = maxParens;
        assignRightParens(parenListTop, maxParens);
        buildParenList(parenListTop, 0, maxParens);
        pnp = parenListTop;

        while ((pnp = nextParenNode(pnp)) != NULL) 
        {
                Node *  np;
                int     value;
                int     i;

                /* set the parentesis in the expression */
                for (i = 0, np = exprNodes; i < maxParens; i++, np = np->next)
                {
                        np->leftParens = pnp->Parens[i].left;
                        np->rightParens = pnp->Parens[i].right;
#ifdef DEBUG
printf("(%d %d) ", np->leftParens, np->rightParens);
#endif
                }

                /* compute the value of the permutation */
                value = computeExpr(exprNodes);
#ifdef DEBUG
printf("value %d\n", value);
#endif

                if (!findValue(value, valueListTop))
                {
                        valueList = insertValue(value, valueList);
                        if (valueListTop == NULL && valueList != NULL)
                                valueListTop = valueList;
                }
        }

        freeNodes(exprNodes);

        /* all done, print the unique value list */
        printValueList(valueListTop);
        freeValueList(valueListTop);
}

int main(int argc, char ** argv)
{
        if (--argc == 1)
                parenPermutations(argv[1]);
		else
				printf("usage: %s expression\n", argv[0]);

		return 0;
}

