#include <bits/stdc++.h>
using namespace std;

struct Task {
    int id;
    string title, desc, date;
    int priority;
    vector<string> tags;
    bool done;
};

vector<Task> tasks;

int idCount = 1;

unordered_map<int, Task> taskmap;

stack<string> undoStack;

queue<string> notify;

list<string> history;

map<int, vector<int>> depGraph;

// func to split tag
vector<string> splittags(string str) {
    vector<string> taglist;
    stringstream ss(str);
    string word;
    while (getline(ss, word, ',')) taglist.push_back(word);
    return taglist;
}

// add task
void addtask() {
    Task t;
    t.id = idCount++;
    cout << "Enter title: "; cin.ignore(); getline(cin, t.title);
    cout << "Enter description: "; getline(cin, t.desc);
    cout << "Enter due date (YYYY-MM-DD): "; getline(cin, t.date);
    cout << "Priority (1-5): "; cin >> t.priority;
    cout << "Tags (comma sep): ";
    cin.ignore(); string tg; getline(cin, tg);
    t.tags = splittags(tg);
    t.done = false;
    tasks.push_back(t);
    taskmap[t.id] = t;
    string msg = "Task added: " + t.title;
    undoStack.push(msg);
    notify.push("New Task: " + t.title);
    time_t now = time(0);
    history.push_back("Added task at " + string(ctime(&now)));
    cout << "\nTask Added with ID: " << t.id << endl;
}

// show tasks
void showtask() {
    for (auto &t : tasks) {
        cout << "ID: " << t.id << endl;
        cout << "Title: " << t.title << endl;
        cout << "Description: " << t.desc << endl;
        cout << "Date: " << t.date << endl;
        cout << "Priority: " << t.priority << endl;
        cout << "Tags: ";
        for (auto &tag : t.tags) cout << tag << " ";
        cout << endl;
        cout << "Status: " << (t.done ? "Done" : "Pending") << "\n----------------\n";
    }
}
//find tasks
void findtask() {
    int id;
    cout << "Enter ID: ";
    cin >> id;

    if (taskmap.count(id)) {
        Task t = taskmap[id];
        cout << "Task Found:\n";
        cout << "ID: " << t.id << endl;
        cout << "Title: " << t.title << endl;
        cout << "Description: " << t.desc << endl;
        cout << "Date: " << t.date << endl;
        cout << "Priority: " << t.priority << endl;
        cout << "Tags: ";
        for (auto &tag : t.tags) cout << tag << " ";
        cout << endl;
        cout << "Status: " << (t.done ? "Done" : "Pending") << endl;
    } else {
        cout << "Not Found\n";
    }
}

// mark done
void markcomplete() {
    int id; cout << "Task ID to mark done: "; cin >> id;
    for (auto &t : tasks) {
        if (t.id == id) {
            t.done = true;
            notify.push("Completed Task: " + t.title);
            undoStack.push("Completed: " + t.title);
            cout << "Marked Done\n";
            return;
        }
    }
    cout << "Task Not Found\n";
}

// delete task
void deletetask() {
    int id; cout << "Enter ID to delete: "; cin >> id;
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->id == id) {
            undoStack.push("Deleted: " + it->title);
            taskmap.erase(id);
            tasks.erase(it);
            cout << "Deleted\n";
            return;
        }
    }
    cout << "Not Found\n";
}

// undo
void undo() {
    if (undoStack.empty()) cout << "Nothing to undo\n";
    else {
        cout << "Undo: " << undoStack.top() << endl;
        undoStack.pop();
    }
}

// sorting
bool sortbydate(Task &a, Task &b) { return a.date < b.date; }
bool sortbyprio(Task &a, Task &b) { return a.priority < b.priority; }

// Swap
void swapTask(Task &a, Task &b) {
    Task temp = a;
    a = b;
    b = temp;
}

// Selection sort
void selectionSort(vector<Task> &tasks, bool (*comp)(Task&, Task&)) {
    int n = tasks.size();
    for (int i = 0; i < n-1; i++) {
        int smallestAt = i;
        for (int j = i+1; j < n; j++) {
            if (comp(tasks[j], tasks[smallestAt])) {
                smallestAt = j;
            }
        }
        if(smallestAt!= i) swapTask(tasks[i], tasks[smallestAt]);
    }
}

// Bubble sort
void bubbleSort(vector<Task> &tasks, bool (*comp)(Task&, Task&)) {
    int n = tasks.size();
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (comp(tasks[j+1], tasks[j])) {
                swapTask(tasks[j], tasks[j+1]);
            }
        }
    }
}

// Insertion sort
void insertionSort(vector<Task> &tasks, bool (*comp)(Task&, Task&)) {
    int n = tasks.size();
    for (int i = 1; i < n; i++) {
        Task key = tasks[i];
        int j = i - 1;
        while (j >= 0 && comp(key, tasks[j])) {
            tasks[j+1] = tasks[j];
            j--;
        }
        tasks[j+1] = key;
    }
}



void sorttask() {
    int sortChoice, algo;
    cout << "Sort by? 1.Date 2.Priority: ";
    cin >> sortChoice;
    cout << "Choose Algorithm: 1.Selection 2.Bubble 3.Insertion: ";
    cin >> algo;

    bool (*comp)(Task&, Task&) = nullptr;

    if (sortChoice == 1) comp = sortbydate;
    else if (sortChoice == 2) comp = sortbyprio;
    else {
        cout << "Invalid sort criteria\n";
        return;
    }

    switch (algo) {
        case 1:
            selectionSort(tasks, comp);
            break;
        case 2:
            bubbleSort(tasks, comp);
            break;
        case 3:
            insertionSort(tasks, comp);
            break;
        default:
            cout << "Invalid algorithm choice\n";
            return;
    }

    cout << "Tasks sorted successfully.\n";
}


// show history
void showhistory() {
    for (auto h : history) cout << h;
}

// show notify
void shownotify() {
    while (!notify.empty()) {
        cout << notify.front() << endl;
        notify.pop();
    }
}

// add dependency
void adddep() {
    int u, v;
    cout << "Enter 2 task IDs: ";
    cin >> u >> v;
    depGraph[u].push_back(v);
    depGraph[v].push_back(u);
    cout << "Dependency added\n";
}

// show deps
void dfs(int u, set<int>& vis) {
    vis.insert(u);
    cout << u << " ";
    for (auto v : depGraph[u]) {
        if (!vis.count(v)) dfs(v, vis);
    }
}

void showdeps() {
    int id; cout << "Task ID: "; cin >> id;
    set<int> vis;
    cout << "Dependent tasks: ";
    dfs(id, vis);
    cout << endl;
}

// Kruskal
struct Edge {
    int u, v, w;
    bool operator<(Edge const& other) { return w < other.w; }
};

vector<Edge> makeedges() {
    vector<Edge> edges;
    for (auto &[u, lst] : depGraph) {
        for (int v : lst) {
            int w = abs(taskmap[u].priority - taskmap[v].priority);
            if (u < v) edges.push_back({u, v, w});
        }
    }
    return edges;
}

int find(int u, vector<int>& par) {
    return par[u] == u ? u : par[u] = find(par[u], par);
}

void join(int u, int v, vector<int>& par) {
    par[find(u, par)] = find(v, par);
}

void kruskal() {
    auto edges = makeedges();
    sort(edges.begin(), edges.end());
    vector<int> par(1000);
    iota(par.begin(), par.end(), 0);
    cout << "MST Edges (u v w):\n";
    for (auto e : edges) {
        if (find(e.u, par) != find(e.v, par)) {
            join(e.u, e.v, par);
            cout << e.u << " " << e.v << " " << e.w << endl;
        }
    }
}

// filter tag
void filtertag() {
    string tg;
    cout << "Tag to search: ";
    cin >> tg;
    for (auto &t : tasks) {
        if (find(t.tags.begin(), t.tags.end(), tg) != t.tags.end()) {
            cout << "ID: " << t.id << " - " << t.title << endl;
        }
    }
}

int main() {
    while (1) {
        cout << "\n--- Task Management system ---" << endl;
        cout << "1. Add new task"<<endl;
        cout <<"2. See all task"<<endl;
        cout <<"3. Find task by ID"<<endl;
        cout<<"4. Mark task done"<<endl;
        cout<<"5. Delete task"<<endl;
        cout<<"6. Undo"<<endl;
        cout<<"7. Sort task"<<endl;
        cout<<"8. History"<<endl;
        cout<<"9. Notification"<<endl;
        cout<<"10. Add dependency"<<endl;
        cout<<"11. Show Dependent Tasks[dfs]"<<endl;
        cout<<"12. Show Task Dependency[krushkal]"<<endl;
        cout<<"13. Filter by tag"<<endl;
        cout<<"0. Exit"<<endl;
        int ch;
        cout << "Enter your choice: ";
        cin >> ch;
        if (ch == 0) break;
        if (ch == 1) addtask();
        else if (ch == 2) showtask();
        else if (ch == 3) findtask();
        else if (ch == 4) markcomplete();
        else if (ch == 5) deletetask();
        else if (ch == 6) undo();
        else if (ch == 7) sorttask();
        else if (ch == 8) showhistory();
        else if (ch == 9) shownotify();
        else if (ch == 10) adddep();
        else if (ch == 11) showdeps();
        else if (ch == 12) kruskal();
        else if (ch == 13) filtertag();
        else cout << "Invalid\n";
    }
    return 0;
}
