#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
using namespace std;
const string SATURDAY = "SAT";
const string SUNDAY = "SUN";
const string MONDAY = "MON";
const string TUESDAY = "TUE";
const string WEDNESDAY = "WED";
const string THURSDAY = "THU";
const string FRIDAY = "FRI";
const char UNDERLINE = '_';
const char PLUS = '+';
const char DASH = '-';
const char VERTICAL_BAR = '|';
const char SPACE = ' ';
const int first_hour = 7;
const int last_hour = 20;
const int characters_in_each_block = 10;
struct course_info {
    string course_name;
    string start;
    int start_num;
    string finish;
    int finish_num;
};
void extract_courses_ID (map <string, string> &courses, char* input_file_path);
void get_lessons (vector <string>& lines);
void setup_weekly_schedule(map <string, string> courses, vector <string> lines, vector < vector<course_info> > &days_of_the_week, char* prog_name);
void fill_new_course_info(string course_name, string group, string day, string start, string finish, vector < vector<course_info> > &days_of_the_week);
void print_weekly_schedule(vector < vector<course_info> > days_of_the_week, char* prog_name);
int set_time(string hour, string minute);
void print_courses_of_the_day(vector < vector <course_info> > sorted_courses);
bool check_interference(struct course_info course, vector <course_info> courses_line);
void sort_courses_of_the_day(vector <course_info> unsorted_courses, vector< vector <course_info> > &sorted_courses);
void print_header_of_the_days();
void draw_course_box_top_and_bottom(vector <course_info> schedule_line);
void draw_course_box_middle(vector <course_info> schedule_line);

int main(int argc, char* argv[]) {
    map <string, string> courses;
    vector <string> lines;
    vector < vector<course_info> > days_of_the_week(7);
    extract_courses_ID(courses, argv[1]);
    get_lessons(lines);
    setup_weekly_schedule(courses, lines, days_of_the_week, argv[2]);
    return 0;
}

void extract_courses_ID (map <string, string> &courses, char* input_file_path) {
    ifstream input_file(input_file_path);
    string line;
    getline(input_file, line);
    while(!input_file.eof()) {
        getline(input_file, line);
        int comma_pos = line.find_first_of(',');
        string ID = line.substr(0, comma_pos);
        string course_name = line.substr(comma_pos + 1);
        courses[ID] = course_name;
    }
}

void get_lessons (vector <string>& lines) {
    string line;
    while (getline(cin, line))
        lines.push_back(line);
}

void setup_weekly_schedule(map <string, string> courses, vector <string> lines, vector < vector<course_info> > &days_of_the_week, char* prog_name) {
    for(int i = 0; i < lines.size(); i++) {
        string command = lines[i];
        size_t dash_pos = command.find_first_of('-');
        string ID = command.substr(0, dash_pos);
        command = command.substr(dash_pos + 1);
        size_t space_pos = command.find_first_of(' ');
        string group = command.substr(0, space_pos);
        command = command.substr(space_pos + 1);
        string course_name = courses[ID];
        dash_pos = command.find_first_of('-');
        while (dash_pos != string::npos) {
            space_pos = command.find_first_of(' ');
            string day = command.substr(0,space_pos);
            command = command.substr(space_pos + 1);
            dash_pos = command.find_first_of('-');
            string start_time = command.substr(0, dash_pos);
            command = command.substr(dash_pos + 1);
            space_pos = command.find_first_of(' ');
            string finish_time;
            if (space_pos == string::npos) finish_time = command;
            else {
                finish_time = command.substr(0, space_pos);
                command = command.substr(space_pos + 1);
            }
            fill_new_course_info(course_name, group, day, start_time, finish_time, days_of_the_week);
            size_t dash_pos = command.find_first_of('-');
        }
    }
    print_weekly_schedule(days_of_the_week, prog_name);
}

void fill_new_course_info(string course_name, string group, string day, string start, string finish, vector < vector<course_info> > &days_of_the_week) {
    struct course_info new_struct;
    string name = course_name;
    if (group[0] == '0') group.erase(0,1);
    string course_full_name = course_name + " (" + group + ')';
    string start_hour = start.substr(0,2);
    string start_minute = start.substr(3,2);
    string finish_hour = finish.substr(0,2);
    string finish_minute = finish.substr(3,2);
    new_struct.course_name = course_full_name;
    new_struct.start_num = set_time(start_hour, start_minute);
    new_struct.finish_num = set_time(finish_hour, finish_minute);
    new_struct.start = start;
    new_struct.finish = finish;
    if (day == SATURDAY)
        days_of_the_week[0].push_back(new_struct);
    else if (day == SUNDAY)
        days_of_the_week[1].push_back(new_struct);
    else if (day == MONDAY)
        days_of_the_week[2].push_back(new_struct);
    else if (day == TUESDAY)
        days_of_the_week[3].push_back(new_struct);
    else if (day == WEDNESDAY)
        days_of_the_week[4].push_back(new_struct);
    else if (day == THURSDAY)
        days_of_the_week[5].push_back(new_struct);
    else if (day == FRIDAY)
        days_of_the_week[6].push_back(new_struct);
}

void print_weekly_schedule(vector < vector<course_info> > days_of_the_week, char* prog_name) {
    cout << "# " << prog_name << endl << endl;
    vector < vector < vector<course_info> > > sorted_courses_of_the_week(7);
    cout << "## Saturday\n\n";
    print_header_of_the_days();
    sort_courses_of_the_day(days_of_the_week[0], sorted_courses_of_the_week[0]);
    print_courses_of_the_day(sorted_courses_of_the_week[0]);
    cout << endl;
    cout << "## Sunday\n\n";
    print_header_of_the_days();
    sort_courses_of_the_day(days_of_the_week[1], sorted_courses_of_the_week[1]);
    print_courses_of_the_day(sorted_courses_of_the_week[1]);
    cout << endl;
    cout << "## Monday\n\n";
    print_header_of_the_days();
    sort_courses_of_the_day(days_of_the_week[2], sorted_courses_of_the_week[2]);
    print_courses_of_the_day(sorted_courses_of_the_week[2]);
    cout << endl;
    cout << "## Tuesday\n\n";
    print_header_of_the_days();
    sort_courses_of_the_day(days_of_the_week[3], sorted_courses_of_the_week[3]);
    print_courses_of_the_day(sorted_courses_of_the_week[3]);
    cout << endl;
    cout << "## Wednesday\n\n";
    print_header_of_the_days();
    sort_courses_of_the_day(days_of_the_week[4], sorted_courses_of_the_week[4]);
    print_courses_of_the_day(sorted_courses_of_the_week[4]);
    cout << endl;
    cout << "## Thursday\n\n";
    print_header_of_the_days();
    sort_courses_of_the_day(days_of_the_week[5], sorted_courses_of_the_week[5]);
    print_courses_of_the_day(sorted_courses_of_the_week[5]);
    cout << endl;
    cout << "## Friday\n\n";
    print_header_of_the_days();
    sort_courses_of_the_day(days_of_the_week[6], sorted_courses_of_the_week[6]);
    print_courses_of_the_day(sorted_courses_of_the_week[6]);
    cout << endl;
}

void print_courses_of_the_day(vector < vector <course_info> > sorted_courses) {
    for (int i = 0; i < sorted_courses.size(); i++) {
        draw_course_box_top_and_bottom(sorted_courses[i]);
        draw_course_box_middle(sorted_courses[i]);
        draw_course_box_top_and_bottom(sorted_courses[i]);
    }
}

bool check_interference(struct course_info course, vector <course_info> courses_line) {
    for (int i = 0; i < courses_line.size(); i++) {
        struct course_info compare = courses_line[i];
        if (!(course.start_num >= compare.finish_num || course.finish_num <= compare.start_num)) {
            return true;
        }
    }
    return false;
}
void sort_courses_of_the_day(vector <course_info> unsorted_courses, vector< vector <course_info> > &sorted_courses) {
    int start_point = 0;
    int lines = 0;
    vector <course_info> new_vector;
    sorted_courses.push_back(new_vector);
    for (int i = 0; i < unsorted_courses.size(); i++) {
        struct course_info course = unsorted_courses[i];
        if (check_interference(course, sorted_courses[lines])) {
            vector <course_info> new_vector;
            new_vector.push_back(course);
            sorted_courses.push_back(new_vector);
            lines++;
        }
        else {
            if (lines == 0) sorted_courses[0].push_back(course);
            else {
                int insertion_line = lines - 1;
                while (!check_interference(course, sorted_courses[insertion_line])) {
                    if (insertion_line != 0) insertion_line--;
                    else {
                        insertion_line = -1;
                        break;
                    }
                }
                sorted_courses[insertion_line + 1].push_back(course);
            }
        }
    }
    for (int i = 0; i < sorted_courses.size(); i++) {
        for (int j = 0; j < sorted_courses[i].size(); j++) {
            for (int k = sorted_courses[i].size() - 1; k > j; k--) {
                if(sorted_courses[i][k].start_num < sorted_courses[i][k - 1].start_num) swap(sorted_courses[i][k], sorted_courses[i][k - 1]);
            }
        }
    }
}

int set_time(string hour, string minute) {
    int integer_hour = stoi(hour);
    int time_turn = 2 * (integer_hour - first_hour);
    if (minute == "30") time_turn++;
    return time_turn;
}

void print_header_of_the_days() {
    for (int hour = first_hour; hour <= last_hour; hour++) {
        if (hour < 10) cout << 0;
        cout << hour << ":00";
        for (int i = 0; i < characters_in_each_block / 2; i++) cout << SPACE;
        if (hour < 10) cout << 0;
        cout << hour << ":30";
        if (hour != last_hour)
            for (int i = 0; i < characters_in_each_block / 2; i++) cout << SPACE;
    }
    cout << endl;
    for (int i = 0; i < characters_in_each_block * 2 * (last_hour - first_hour) + characters_in_each_block + characters_in_each_block / 2; i++) cout << UNDERLINE;
    cout << endl;
}

void draw_course_box_top_and_bottom(vector <course_info> schedule_line) {
    if (schedule_line.size() != 0) {
        cout << "  ";
        for (int course = 0; course < schedule_line.size(); course++) {
            if (course == 0)
                for (int i = 0; i < characters_in_each_block * schedule_line[course].start_num; i++) cout << SPACE;
            else
                for (int i = 0; i < characters_in_each_block * (schedule_line[course].start_num - schedule_line[course - 1].finish_num); i++) cout << SPACE;
            cout << PLUS;
            for (int i = 0; i < characters_in_each_block * (schedule_line[course].finish_num - schedule_line[course].start_num) - 2; i++) cout << DASH;
            cout << PLUS;
        }
        cout << endl;
    }
}

void draw_course_box_middle(vector <course_info> schedule_line) {
    if (schedule_line.size() != 0) {
        cout << "  ";
        for (int course = 0; course < schedule_line.size(); course++) {
            if (course == 0)
                for (int i = 0; i < characters_in_each_block * schedule_line[course].start_num; i++) cout << SPACE;
            else
                for (int i = 0; i < characters_in_each_block * (schedule_line[course].start_num - schedule_line[course - 1].finish_num); i++) cout << SPACE;
            cout << VERTICAL_BAR;
            int name_length = schedule_line[course].course_name.length();
            int num_of_spaces = characters_in_each_block * (schedule_line[course].finish_num - schedule_line[course].start_num) - name_length;
            for (int i = 1; i < (num_of_spaces + 1) / 2; i++) cout << SPACE;
            cout << schedule_line[course].course_name;
            for (int i = 1; i < (num_of_spaces) / 2; i++) cout << SPACE;
            cout << VERTICAL_BAR;
        }
        cout << endl;
    }
}
