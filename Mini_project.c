#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_NAME_LENGTH 50

// Structure for a User Profile
typedef struct User {
    int id;
    char name[MAX_NAME_LENGTH];
    char bio[100];
} User;

// Structure for the Graph
typedef struct Graph {
    int numUsers;
    int adjMatrix[MAX_USERS][MAX_USERS];
    User users[MAX_USERS];
} Graph;

// Function to initialize the graph
void initializeGraph(Graph* graph, int numUsers) {
    graph->numUsers = numUsers;
    for (int i = 0; i < numUsers; i++) {
        for (int j = 0; j < numUsers; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

// Function to create a user profile and save it to a file
void createUser(Graph* graph, int id, char name[], char bio[]) {
    graph->users[id].id = id;
    strcpy(graph->users[id].name, name);
    strcpy(graph->users[id].bio, bio);

    // Save user information to the file
    FILE* file = fopen("social_network_data.txt", "a");
    if (file) {
        fprintf(file, "User Created: ID=%d, Name=%s, Bio=%s\n", id, name, bio);
        fclose(file);
    } else {
        printf("Error: Unable to open file for writing.\n");
    }
}

// Function to display a user's profile
void displayUser(Graph* graph, int id) {
    printf("User ID: %d\n", graph->users[id].id);
    printf("Name: %s\n", graph->users[id].name);
    printf("Bio: %s\n", graph->users[id].bio);
}

// Function to add a friendship and save it to a file
void addFriendship(Graph* graph, int user1, int user2) {
    graph->adjMatrix[user1][user2] = 1;
    graph->adjMatrix[user2][user1] = 1;

    // Save friendship information to the file
    FILE* file = fopen("social_network_data.txt", "a");
    if (file) {
        fprintf(file, "Friendship Added: %s <--> %s\n", graph->users[user1].name, graph->users[user2].name);
        fclose(file);
    } else {
        printf("Error: Unable to open file for writing.\n");
    }
}

// Function to find mutual friends between two users
void mutualFriends(Graph* graph, int user1, int user2) {
    printf("Mutual friends between %s and %s: ", graph->users[user1].name, graph->users[user2].name);
    int found = 0;
    for (int i = 0; i < graph->numUsers; i++) {
        if (graph->adjMatrix[user1][i] && graph->adjMatrix[user2][i]) {
            printf("%s ", graph->users[i].name);
            found = 1;
        }
    }
    if (!found) {
        printf("None");
    }
    printf("\n");
}

// Function to find the shortest path between two users using BFS
void shortestPath(Graph* graph, int start, int end) {
    int visited[MAX_USERS] = {0}, queue[MAX_USERS], distance[MAX_USERS];
    int front = 0, rear = 0;
    int parent[MAX_USERS];
    for (int i = 0; i < MAX_USERS; i++) {
        parent[i] = -1;
        distance[i] = -1;
    }

    visited[start] = 1;
    distance[start] = 0;
    queue[rear++] = start;

    while (front < rear) {
        int current = queue[front++];
        for (int i = 0; i < graph->numUsers; i++) {
            if (graph->adjMatrix[current][i] && !visited[i]) {
                visited[i] = 1;
                parent[i] = current;
                distance[i] = distance[current] + 1;
                queue[rear++] = i;
                if (i == end) break;
            }
        }
    }

    if (distance[end] == -1) {
        printf("No path between %s and %s\n", graph->users[start].name, graph->users[end].name);
    } else {
        printf("Shortest path between %s and %s: %d steps\n", graph->users[start].name, graph->users[end].name, distance[end]);
        printf("Path: ");
        int path[MAX_USERS], pathIndex = 0;
        for (int v = end; v != -1; v = parent[v]) {
            path[pathIndex++] = v;
        }
        for (int i = pathIndex - 1; i >= 0; i--) {
            printf("%s ", graph->users[path[i]].name);
        }
        printf("\n");
    }
}

// Function to display the file contents
void viewFileContents() {
    FILE* file = fopen("social_network_data.txt", "r");
    if (file) {
        char line[256];
        printf("\n--- Social Network Data ---\n");
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        printf("Error: Unable to open file for reading.\n");
    }
}

// Function to display the menu
void displayMenu() {
    printf("\n---> Social Network Graph Analysis <---\n");
    printf("1. Create User\n");
    printf("2. Add Friendship\n");
    printf("3. View User Profile\n");
    printf("4. Find Mutual Friends\n");
    printf("5. Find Shortest Path\n");
    printf("6. View Saved Data\n");
    printf("7. Exit\n");
    printf("Choose an option: ");
}

// Main function
int main() {
    Graph graph;
    int numUsers = MAX_USERS;  // Initialize with the maximum allowed users
    int choice, user1, user2;
    char name[MAX_NAME_LENGTH], bio[100];

    initializeGraph(&graph, numUsers);

    // Main loop for menu
    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter user ID: ");
                scanf("%d", &user1);
                if (user1 >= numUsers) {
                    printf("Invalid user ID.\n");
                    break;
                }
                printf("Enter name: ");
                scanf(" %[^\n]", name);
                printf("Enter bio: ");
                scanf(" %[^\n]", bio);
                createUser(&graph, user1, name, bio);
                printf("User created successfully!\n");
                break;
            case 2:
                printf("Enter two users to add a friendship (user1 user2): ");
                scanf("%d %d", &user1, &user2);
                if (user1 >= numUsers || user2 >= numUsers) {
                    printf("Invalid user IDs.\n");
                    break;
                }
                addFriendship(&graph, user1, user2);
                printf("Friendship added successfully!\n");
                break;
            case 3:
                printf("Enter user ID to view profile: ");
                scanf("%d", &user1);
                if (user1 >= numUsers) {
                    printf("Invalid user ID.\n");
                    break;
                }
                displayUser(&graph, user1);
                break;
            case 4:
                printf("Enter two users to find mutual friends (user1 user2): ");
                scanf("%d %d", &user1, &user2);
                if (user1 >= numUsers || user2 >= numUsers) {
                    printf("Invalid user IDs.\n");
                    break;
                }
                mutualFriends(&graph, user1, user2);
                break;
            case 5:
                printf("Enter two users to find the shortest path (user1 user2): ");
                scanf("%d %d", &user1, &user2);
                if (user1 >= numUsers || user2 >= numUsers) {
                    printf("Invalid user IDs.\n");
                    break;
                }
                shortestPath(&graph, user1, user2);
                break;
            case 6:
                viewFileContents();
                return 0;
            case 7:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}
