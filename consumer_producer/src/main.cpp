#include <condition_variable>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <mutex>
#include <random>
#include <thread>

#define MAX_ITEMS 10

std::list<int> items;              // our shared variable
std::mutex items_mutex;            // our mutex to lock the shared variable
std::condition_variable items_cv;  // our conditional variable for inter thread communication

int produce_item() {
  /*std::srand(std::time(0));
  int item = std::rand();*/
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dist(1.0, 99.0);

  int item = dist(mt);
  std::cout << "produce item: " << item << std::endl;
  return item;
}

void consume_item(int item) { std::cout << "consume item: " << item << std::endl; }

void producer_fn() {
  for (int n = 0; n < 10; n++) {
    // produce items
    {
      // lock the access to items. after exiting the scope,
      // the guard unlocks automatically the mutex.
      std::lock_guard<std::mutex> guard(items_mutex);
      for (int i = 0; i < MAX_ITEMS; i++) {
        auto item = produce_item();
        items.push_back(item);
      }
    }

    // notify consumer
    std::cout << "producer_fn: notify consumer" << std::endl;
    items_cv.notify_one();  // tell our consumer that he can consume items

    // wait unitl all items are consumed
    {
      std::cout << "producer_fn: wait for notificatiom from consumer" << std::endl;
      std::unique_lock<std::mutex> lock(items_mutex);
      items_cv.wait(lock, [] {
        return items.size() == 0;  // our condition is an empty list
      });
    }
  }
}

void consumer_fn() {
  for (int n = 0; n < 10; n++) {
    // wait until producer produced items
    std::cout << "consumer_fn: wait for notification from producer" << std::endl;
    std::unique_lock<std::mutex> lock(items_mutex);
    items_cv.wait(lock, [] {
      return items.size() == MAX_ITEMS;  // our condition is a full list
    });

    // consume all items and clear the list
    for (auto item : items) {
      consume_item(item);
    }
    items.clear();

    // lock.unlock(); // IMHO not neccessary, because the unique_lock will
    // expire after exiting the for loop scope And the producer
    // tries to aquire an lock for this mutex.

    // notify producder
    std::cout << "consumer_fn: notify producer" << std::endl;
    items_cv.notify_one();
  }
}

int main() {
  std::thread producer_thread(producer_fn);
  std::thread consumer_thread(consumer_fn);

  producer_thread.join();
  consumer_thread.join();

  return 0;
}
