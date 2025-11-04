#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>

using namespace std;

// ===============================================
// 1. GLOBALA ENUM-KLASSER
// ===============================================

enum class Intent_of_direction { Left, Straight, Right };
enum class Light_Color { Red, Yellow, Green };

// Ny Enum: För Intersection Cycle State
enum class Intersection_State { North_South_Green, North_South_Yellow, East_West_Green, East_West_Yellow };

// ===============================================
// 2. KLASS: VEHICLE (BIL)
// ===============================================

class Vehicle {
private:
    int individual_car;
    double current_speed_on_road;
    Intent_of_direction intent_direction;

public:
    Vehicle(int id, double current_speed, Intent_of_direction direction) :
        individual_car (id), current_speed_on_road (current_speed), intent_direction (direction)
    {} 

    Intent_of_direction get_avsikt() const { return intent_direction; }
    string to_string() const {
        string intent_str;
        if (intent_direction == Intent_of_direction::Left) intent_str = "Vänster";
        else if (intent_direction == Intent_of_direction::Straight) intent_str = "Rakt Fram";
        else intent_str = "Höger";
        return "[Bil ID: " + std::to_string(individual_car) + ", Avsikt: " + intent_str + "]";
    }
};

// ===============================================
// 3. KLASS: TRAFFICLIGHT (Förenklad, styrs av Intersection)
// ===============================================

class TrafficLight {
private:
    Light_Color current_color;

public:
    TrafficLight() : current_color(Light_Color::Red) {}

    Light_Color get_color() const { return current_color; }
    
    // NY FUNKTION: För Intersection att tvinga fram en färg
    void set_color(Light_Color new_color) { current_color = new_color; }
};

// ===============================================
// 4. KLASS: ROADS (VÄGAR)
// ===============================================

class Roads {
private:
    string name;
    queue<Vehicle> left_lane;
    queue<Vehicle> straight_right_lane;
    TrafficLight light; 

public:
    Roads(string n) : name(n) {}

    void add_vehicle(const Vehicle& v) {
        if (v.get_avsikt() == Intent_of_direction::Left) {
            left_lane.push(v);
        } else {
            straight_right_lane.push(v);
        }
    }
    
    TrafficLight& get_light() { return light; } 
    queue<Vehicle>& get_left_lane() { return left_lane; }
    queue<Vehicle>& get_straight_right_lane() { return straight_right_lane; }
    string get_name() const { return name; }
    bool has_vehicles() const { return !left_lane.empty() || !straight_right_lane.empty(); }
};

// ===============================================
// 5. KLASS: INTERSECTION (KONTROLLCENTRALEN)
// ===============================================

class Intersection {
private:
    Roads north = Roads("Nord");
    Roads south = Roads("Syd");
    Roads east = Roads("Ost");
    Roads west = Roads("Vast");
    
    // NYA KONTROLLVARIABLER
    Intersection_State current_state = Intersection_State::North_South_Green;
    int state_timer = 0;
    
    // Tidsramar i ticks (sekunder)
    const int NS_GREEN_TIME = 8;
    const int EW_GREEN_TIME = 8;
    const int YELLOW_TIME = 2;

    void update_lights() {
        // Kontrollerar ljus baserat på Intersection_State
        Light_Color ns_color, ew_color;

        if (current_state == Intersection_State::North_South_Green) {
            ns_color = Light_Color::Green; ew_color = Light_Color::Red;
        } else if (current_state == Intersection_State::North_South_Yellow) {
            ns_color = Light_Color::Yellow; ew_color = Light_Color::Red;
        } else if (current_state == Intersection_State::East_West_Green) {
            ns_color = Light_Color::Red; ew_color = Light_Color::Green;
        } else { // East_West_Yellow
            ns_color = Light_Color::Red; ew_color = Light_Color::Yellow;
        }

        north.get_light().set_color(ns_color);
        south.get_light().set_color(ns_color);
        east.get_light().set_color(ew_color);
        west.get_light().set_color(ew_color);
    }

    void handle_traffic(Roads& primary_road, Roads& opposing_road) {
        // Logik för RAKT FRAM och HÖGER (konfliktfritt)
        if (primary_road.get_light().get_color() == Light_Color::Green) {
            if (!primary_road.get_straight_right_lane().empty()) {
                Vehicle v = primary_road.get_straight_right_lane().front();
                primary_road.get_straight_right_lane().pop();
                cout << "   - " << primary_road.get_name() << " kör RAKT/HÖGER: " << v.to_string() << endl;
            }

            // Logik för VÄNSTER (KONTROLL AV KONFLIKT)
            if (!primary_road.get_left_lane().empty()) {
                // Denna logik är förenklad: låt vänstersväng köra om RAKT fram filen är tom
                // En mer avancerad version skulle kontrollera mötande rakt fram trafik.
                if (primary_road.get_straight_right_lane().empty()) { 
                    Vehicle v = primary_road.get_left_lane().front();
                    primary_road.get_left_lane().pop();
                    cout << "   - " << primary_road.get_name() << " kör VÄNSTER (Fil tom): " << v.to_string() << endl;
                }
            }
        }
    }

public:
    Intersection() {
        // Initiera några bilar
        north.add_vehicle(Vehicle(101, 50.0, Intent_of_direction::Straight));
        north.add_vehicle(Vehicle(102, 50.0, Intent_of_direction::Left));
        north.add_vehicle(Vehicle(103, 50.0, Intent_of_direction::Straight));
        south.add_vehicle(Vehicle(201, 50.0, Intent_of_direction::Straight));
        east.add_vehicle(Vehicle(301, 50.0, Intent_of_direction::Straight));
        west.add_vehicle(Vehicle(401, 50.0, Intent_of_direction::Left));
    }

    void process_tick() {
        state_timer++;
        
        // --- 1. TIDSSTYRNING (STATE MACHINE) ---
        if (current_state == Intersection_State::North_South_Green && state_timer > NS_GREEN_TIME) {
            current_state = Intersection_State::North_South_Yellow;
            state_timer = 0;
            cout << "   [CYKELBYTE] Nord/Syd byter till GULT.\n";
        } else if (current_state == Intersection_State::North_South_Yellow && state_timer > YELLOW_TIME) {
            current_state = Intersection_State::East_West_Green;
            state_timer = 0;
            cout << "   [CYKELBYTE] Öst/Väst byter till GRÖNT.\n";
        } else if (current_state == Intersection_State::East_West_Green && state_timer > EW_GREEN_TIME) {
            current_state = Intersection_State::East_West_Yellow;
            state_timer = 0;
            cout << "   [CYKELBYTE] Öst/Väst byter till GULT.\n";
        } else if (current_state == Intersection_State::East_West_Yellow && state_timer > YELLOW_TIME) {
            current_state = Intersection_State::North_South_Green;
            state_timer = 0;
            cout << "   [CYKELBYTE] Nord/Syd byter till GRÖNT.\n";
        }

        // 2. TILLÄMPA LJUSINSTÄLLNINGAR
        update_lights();

        // 3. HANTERA BILFLÖDET
        if (current_state == Intersection_State::North_South_Green || current_state == Intersection_State::North_South_Yellow) {
            cout << "-> Nord/Syd Axeln Körs.\n";
            handle_traffic(north, south);
            handle_traffic(south, north);
        } else {
            cout << "-> Öst/Väst Axeln Körs.\n";
            handle_traffic(east, west);
            handle_traffic(west, east);
        }
    }
};

// ===============================================
// 6. KLASS: TRAFFICSYSTEM (HUVUDKONTROLL)
// ===============================================

class TrafficSystem {
private:
    Intersection main_intersection; 
    int current_tick = 0;
    const int MAX_TICKS = 30; // Kör en längre tid för att se cykeln

public:
    TrafficSystem() {}

    void start_simulation() {
        cout << "\n--- STARTAR TRAFIKSIMULERING. Max Tick: " << MAX_TICKS << " ---\n";

        while (current_tick < MAX_TICKS) {
            cout << "\n===============================";
            cout << "\n[TID: " << current_tick << "]\n";
            
            main_intersection.process_tick(); 

            current_tick++;
        }
        cout << "\n--- SIMULERING AVSLUTAD PÅ TID: " << current_tick << " ---\n";
    }
};

// ===============================================
// 7. MAIN-FUNKTIONEN
// ===============================================

int main() {
    TrafficSystem system;
    system.start_simulation();
    return 0;
}