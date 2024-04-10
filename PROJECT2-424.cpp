// PROJECT2-424.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <cmath>
using namespace std;

// Abstract base class for sensors
class Sensor {
public:
    virtual ~Sensor() {}
    virtual void gatherData() const = 0;
    virtual void processData() const = 0;
};

// Derived class for temperature sensor
class TemperatureSensor : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Temperature Sensor." << endl;
    }
    void processData() const override {
        cout << "Processing data from Temperature Sensor." << endl;
    }
};

// Derived class for pressure sensor
class PressureSensor : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Pressure Sensor." << endl;
    }
    void processData() const override {
        cout << "Processing data from Pressure Sensor." << endl;
    }
};

// Derived class for position sensor
class PositionSensor : public Sensor {
public:
    void gatherData() const override {
        cout << "Gathering data from Position Sensor." << endl;
    }
    void processData() const override {
        cout << "Processing data from Position Sensor." << endl;
    }
};

// Sensor Factory class
class SensorFactory {
public:
    static Sensor* createSensor(const std::string& type) {
        if (type == "Temperature")
            return new TemperatureSensor();
        else if (type == "Pressure")
            return new PressureSensor();
        else if (type == "Position")
            return new PositionSensor();
        else
            return nullptr;
    }
};

// Aerospace Control System class
class AerospaceControlSystem {
private:
    vector<Sensor*> sensors;

public:
    void addSensor(Sensor* sensor) {
        sensors.push_back(sensor);
    }

    void monitorAndAdjust() {
        for (auto sensor : sensors) {
            sensor->gatherData();
            sensor->processData();
            cout << "Adjusting controls based on sensor data." << endl;
        }
    }

    ~AerospaceControlSystem() {
        for (auto sensor : sensors) {
            delete sensor;
        }
    }
};


// PROBLEM 2, START CODE
std::recursive_mutex m1, m2;
const int numRobots = 5;
using namespace std;

// set Bool for each tool, true if currently in use
bool tool1 = 0, tool2 = 0, tool3 = 0, tool4 = 0, tool5 = 0;
void performTask(int robotID, int robotID2, int waitorder)
{
    m1.lock();
    int waittime = 6 * waitorder;
    // Each robot picks up the tools next to them
    cout << "Robot " << robotID << " is picking up the tools next to them" << endl;
    cout << "Robot " << robotID2 << " is picking up the tools next to them" << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Pick up tool
    cout << "Robot " << robotID << " has picked up tools, starting to complete task" << endl;
    cout << "Robot " << robotID2 << " has picked up tools, starting to complete task" << endl;

    // Completing the task
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Robot " << robotID << " completed the task.\n";
    std::cout << "Robot " << robotID2 << " completed the task.\n";
    m1.unlock();
}

void performTask1(int robotID, int waitorder)
{
    m1.lock();
    int waittime = 6 * waitorder;
    // Each robot picks up the tools next to them
    cout << "Robot " << robotID << " is picking up the tools next to them" << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Pick up tool
    cout << "Robot " << robotID << " has picked up tools, starting to complete task" << endl;

    // Completing the task
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Robot " << robotID << " completed the task.\n";
    m1.unlock();
}


// problem 3

bool atc_awake = false;
int aircraft_in_pattern = 0;


void pilot_communication(int aircraft_id) {
    m1.lock();
    cout << "Aircraft " << aircraft_id << " is requesting landing." << endl;
    if (!atc_awake) {
        cout << "Aircraft " << aircraft_id << " woke up the ATC." << endl;
        atc_awake = true;
    }
    else {
        if (aircraft_in_pattern < 3) {
            cout << "Aircraft " << aircraft_id << " is cleared to land, entering the traffic pattern." << endl;
            aircraft_in_pattern++;
        }
        else {
            cout << "Apporach Pattern Full. Aircraft " << aircraft_id << " directed to another airport" << endl << "Aircraft " << aircraft_id << " Flying to another airport" << endl;
        }
    }
    m1.unlock();
}


void atc_communication(int aircraft_id) {
    m1.lock();
    if (atc_awake) {
        cout << "ATC is talking to Aircraft " << aircraft_id << endl;
        this_thread::sleep_for(chrono::seconds(1)); // Landing process
        aircraft_in_pattern--;
        cout << "Aircraft " << aircraft_id << " is cleared to land." << endl;
        if (aircraft_in_pattern == 0) {
            cout << "Runway is free." << endl;
            cout << "ATC fell asleep." << endl;
            atc_awake = false;
        }
    }
    m1.unlock();
}

// PROBLEM 4, plot bending mode of free-free beam
// Will be a trig function, sin or cos depending on mode
// scale will depend on many factors, desnsity, stiffness, shape of cross section, 
// to simplify, scale axis to get **FIFTH BENDING MODE SHAPE**
const int WIDTH = 800;
const int HEIGHT = 600;

class Graph {
public:
    Graph() {
        window.create(sf::VideoMode(WIDTH, HEIGHT), "Function Graph");
        window.setFramerateLimit(60);
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            drawGraph();
            window.display();
        }
    }

private:
    sf::RenderWindow window;

    void drawGraph() {
        sf::VertexArray graph(sf::LineStrip, WIDTH);
        graph[0].position = sf::Vector2f(0, HEIGHT / 2);

        for (int i = 0; i < WIDTH; ++i) {
            float x = static_cast<float>(i - WIDTH / 2) / 50; // Scale x-axis
            float y = std::sin(x); // Function to graph

            // Adjust y to fit within window
            y *= 50;
            y += HEIGHT / 2;

            graph[i].position = sf::Vector2f(i, y);
            graph[i].color = sf::Color::Green;
        }

        window.draw(graph);
    }
};




// MAIN Function
int main() {

    cout << "PROBLEM 1:" << endl;

    AerospaceControlSystem ctrlSys;

    // Adding sensors
    ctrlSys.addSensor(SensorFactory::createSensor("Temperature"));
    ctrlSys.addSensor(SensorFactory::createSensor("Pressure"));
    ctrlSys.addSensor(SensorFactory::createSensor("Position"));

    // Monitoring and adjusting
    ctrlSys.monitorAndAdjust();

    cout << endl << "PROBLEM 2:" << endl;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    std::thread robots[numRobots];
    int robot1 = 1, robot2 = 2, robot3 = 3, robot4 = 4, robot5 = 5;
    int order1 = 0, order2 = 1, order3 = 2;
    //generate order of robots
   // define all threads
    thread r1(performTask, robot1, robot3, order1);
    thread r2(performTask, robot2, robot4, order2);
    thread r3(performTask1, robot5, order3);
    r1.join();
    r2.join();
    r3.join();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Total runtime: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds\n";

    cout << endl << "PROBLEM 3: " << endl;

    std::chrono::steady_clock::time_point start2 = std::chrono::steady_clock::now();
    vector<thread> aircraft_threads;
    for (int i = 1; i <= 10; ++i) {
        aircraft_threads.emplace_back([i]() {
            pilot_communication(i);
            this_thread::sleep_for(chrono::milliseconds(100)); // Simulate slight delay before ATC communication
            atc_communication(i);
            });
    }

    for (auto& thread : aircraft_threads) {
        thread.join();
    }
    std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
    std::cout << "Total runtime: " << std::chrono::duration_cast<std::chrono::seconds>(end2 - start2).count() << " seconds\n";


    cout << endl << "PROBLEM 4:" << endl << "Mode Shape of Fourth Bending Mode of Free-Free Beam" << endl;
    Graph graph;
    graph.run();

    return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu


