#ifndef __M_THREADSAFETY_H__
#define __M_THREADSAFETY_H__

/* 
 * definition of thread safe qeue
 * author: Jiri Zahradnik
 * date: spring 2017
 */ 

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace threadSafety{
    template <class T>
    class queue_t{
        private:
            /* methods */

            std::queue<T> m_q; //< @brief thread safe Q
            std::mutex m_mut; //< @brief mutex to ensure thread safety
            std::condition_variable var;
            using lock_t = std::unique_lock<std::mutex>;
            std::atomic<bool> m_empty;

        public:
            /* methods */

            /**
             * @brief constructor of thread safe Q
             */
            queue_t(){
                m_empty = true;
            }

            /**
             * @brief pushes item into Q
             * @param item item of type T that will be pushed in Q
             */
            void push(T &item){
                lock_t m_lock = lock_t(m_mut);
                m_q.push(item);
                m_empty = false;
                var.notify_one();
            }

            /**
             * @brief gets first member from Q and pops it
             * @return first member from Q
             */
            T pop(){
                lock_t m_lock = lock_t(m_mut);
                while(m_empty){
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    var.wait(m_lock);
                }
                T item = std::move(m_q.front());
                m_q.pop();
                if(m_q.empty()){
                    m_empty = true;
                }

                return item;
            }

            /**
             * 
             * @return reference to first item;
             */
            T &front(){
                lock_t m_lock = lock_t(m_mut);
                while(m_empty){
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    var.wait(m_lock);
                }
                T &item = m_q.front();

                return item;
            };
            /**
             * 
             * @return true or false depending on emptyness of the Q
             */
            bool empty(){
                lock_t m_lock = lock_t(m_mut);
                if(m_q.empty()){
                    return true;
                }
                else{
                    return false;
                }
            }

            void clear(){
                lock_t m_lock = lock_t(m_mut);
                while(m_empty){
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    var.wait(m_lock);
                }
                while(!m_q.empty()){
                    m_q.pop();
                }
            }
    };
    
    template <class T>
    class stack_t{
        private:
            std::deque<T> m_dq; //< @brief internal implementation of stack
            std::mutex m_mut; //< @brief mutex to ensure thread safety
            std::condition_variable var;
            using lock_t = std::unique_lock<std::mutex>;
            std::atomic<bool> m_empty;
        
        public:
            
    };
//    template<typename T>
//    class pakos
//    {
//        pakos(std::mutex &m, T& t_): mtx(m), t(t_){}
//        T* operator->() 
//        {
//            return &t;
//        }
//        
//        std::mutex &mtx;
//        T &t;
//        std::unique_lock lck(mtx);
//    };
}

#endif