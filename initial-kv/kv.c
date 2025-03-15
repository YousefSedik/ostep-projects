#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// defining a node
typedef struct Node
{
    char *key;
    char *value;
    struct Node *next;
    struct Node *prev;
} Node;

Node *createNode(char *key, char *value)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->key = key;
    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}
void insertAtBeginning(Node **head, char *key, char *value)
{
    Node *newNode = createNode(key, value);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    newNode->next = *head;
    (*head)->prev = newNode;
    *head = newNode;
}

int deleteAtBeginning(Node **head)
{
    if (*head == NULL)
    {
        return 0;
    }
    Node *temp = *head;
    *head = (*head)->next;
    if (*head != NULL)
    {
        (*head)->prev = NULL;
    }
    free(temp);
    return 1;
}
int deleteKey(Node **head, char *key)
{
    if (*head == NULL)
    {
        return 0;
    }
    Node *temp = *head;
    if (!strcmp(temp->key, key))
    {
        return deleteAtBeginning(head);
    }
    while (temp != NULL && strcmp(temp->key, key))
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return 0;
    }
    if (temp->next != NULL)
    {
        temp->next->prev = temp->prev;
    }
    if (temp->prev != NULL)
    {
        temp->prev->next = temp->next;
    }
    free(temp);
    return 1;
}
int printValue(Node **head, char *key)
{
    Node *temp = *head;
    while (temp != NULL)
    {
        if (!strcmp(temp->key, key))
        {
            printf("%s,%s\n", temp->key, temp->value);
            return 1;
        }
        temp = temp->next;
    }
    return 0;
}
void printListForward(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        printf("%s,%s\n", temp->key, temp->value);
        temp = temp->next;
    }
}
void writeDatabaseFile(Node *head)
{
    Node *temp = head;
    FILE *fp = fopen("database.txt", "wb");
    while (temp != NULL)
    {
        if (!(temp->key && temp->value))
        {
            temp = temp->next;
            continue;
        }
        int len = strlen(temp->key) + strlen(temp->value) + 2;
        char line[len];
        int k = 0;
        for (int i = 0; i < strlen(temp->key); i++)
        {
            line[k] = temp->key[i];
            k++;
        }
        line[k] = ',';
        k++;
        for (int i = 0; i < strlen(temp->value); i++)
        {
            line[k] = temp->value[i];
            k++;
        }
        line[k] = '\n'; // remove the \n
        fwrite(line, sizeof(line[0]), len, fp);
        temp = temp->next;
    }
    fclose(fp);
}
void UpdateOrInsert(Node **head, char *key, char *value)
{
    // search if exists
    Node *temp = *head;
    int is_found = 0;
    while (temp != NULL)
    {
        if (!strcmp(temp->key, key))
        {
            temp->value = value;
            is_found = 1;
        }
        temp = temp->next;
    }
    if (!is_found)
    {
        insertAtBeginning(head, key, value);
    }
}
int main(int argc, char *argv[])
{
    Node *head = NULL;
    short isBadCommand = 0;
    FILE *fp = fopen("database.txt", "r+");
    char *line = NULL;
    size_t len = 0;
    size_t nread;
    if (fp != NULL)
    {
        while ((nread = getline(&line, &len, fp)) != -1)
        {
            char *key = strsep(&line, ",");
            char *value = strsep(&line, ",");
            if (nread >= 3)
            {
                value[strlen(value) - 1] = '\0';
                insertAtBeginning(&head, key, value);
            }
        }
        free(line);
        fclose(fp);
    }
    for (int i = 1; i < argc; i++)
    {
        char *action = strsep(&argv[i], ",");
        if (!strcmp(action, "g"))
        {
            char *key = strsep(&argv[i], ",");
            if (key == NULL)
            {
                isBadCommand = 1;
            }
            else
            {
                int found = printValue(&head, key);
                if (!found)
                {
                    printf("%s not found\n", key);
                }
            }
        }
        else if (!strcmp(action, "p"))
        {
            char *key = strsep(&argv[i], ",");
            char *value = strsep(&argv[i], ",");
            if (key == NULL || value == NULL || !strcmp(value, ""))
            {
                isBadCommand = 1;
            }
            else
            {
                // if key exists, edit it, otherwise insert a new node
                UpdateOrInsert(&head, key, value);
            }
        }
        else if (!strcmp(action, "d"))
        { // delete data
            char *key = strsep(&argv[i], ",");
            if (key == NULL)
            {
                isBadCommand = 1;
            }
            else
            {
                if (!deleteKey(&head, key))
                {
                    printf("%s not found\n", key);
                }
            }
        }
        else if (!strcmp(action, "c"))
        { // clear data
            while (head != NULL)
            {
                deleteAtBeginning(&head);
            }
        }
        else if (!strcmp(action, "a"))
        { // all data
            printListForward(head);
        }
        else
        {
            isBadCommand = 1;
        }
        if (isBadCommand)
        {
            isBadCommand = 0;
            printf("bad command\n");
        }
    }
    // write the database in database.txt
    writeDatabaseFile(head);
}