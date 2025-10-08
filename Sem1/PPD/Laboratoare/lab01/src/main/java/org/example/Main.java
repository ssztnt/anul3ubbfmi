package org.example;

import java.util.Arrays;
import java.util.Random;


public class Main {


    public static void main(String[] args) {

        int VEC_SIZE = 10;

        int a[] = generateArray(VEC_SIZE, 10);
        int b[] = generateArray(VEC_SIZE, 10);
        int[] c_seq = new int[VEC_SIZE];
        int[] c_par = new int[VEC_SIZE];

        long startTime = System.nanoTime();

        for (int i = 0; i < VEC_SIZE; i++) {
            c_seq[i] = a[i] + b[i];
        }

        long endTime = System.nanoTime();

        if (VEC_SIZE < 10) {
            System.out.println(Arrays.toString(a));
            System.out.println(Arrays.toString(b));
            System.out.println(Arrays.toString(c_seq));
        }

        System.out.println("Sequential addition took " + (endTime - startTime) + " nanoseconds.");

        startTime = System.currentTimeMillis();

        int p = 4 , start = 0  , end = VEC_SIZE/p , rest = VEC_SIZE%p;
        MyThread[] threads = new MyThread[p];

        for (int i = 0; i < p; i++) {
            if ( rest > 0) {
                end ++;
                rest --;
            }
            threads[i] = new MyThread(a , b , c_par , start , end);
            threads[i].start();
            start = end;
            end = start + VEC_SIZE/p;

        }

        for (int i = 0; i < p; i++) {
            try{
                threads[i].join();
            }catch (InterruptedException e){
                e.printStackTrace();
            }
        }
        endTime = System.currentTimeMillis();
        if (VEC_SIZE < 10) {
            System.out.println(Arrays.toString(c_par));
        }
        System.out.println("Parallel addition took " + (endTime - startTime) + " nanoseconds.");

        MyThread2[] threads2 = new MyThread2[VEC_SIZE];
        startTime = System.currentTimeMillis();
        for (int i = 0; i < VEC_SIZE; i++) {
            threads2[i] = new MyThread2(a , b , c_par , i , VEC_SIZE , VEC_SIZE);
            threads2[i].start();
        }
        for (int i = 0; i < VEC_SIZE; i++) {
            try{
                threads2[i].join();
            }catch (InterruptedException e){
                e.printStackTrace();
            }
        }
        endTime = System.currentTimeMillis();
        if (VEC_SIZE < 10) {
            System.out.println(Arrays.toString(c_par));
        }
        System.out.println("Parallel addition (one thread per element) took " + (endTime - startTime) + " nanoseconds.");
    }



    // Generates an array of random integers between 0 (inclusive) and upperBound (exclusive)
    public static int[] generateArray(int size, int upperBound) {
        Random rand = new Random();
        int[] arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = rand.nextInt(upperBound);
        }
        return arr;
    }

    public static class MyThread extends Thread {
        private int [] a , b , c ;
        private int start , end ;

        public MyThread(int [] a , int [] b , int [] c ,int start , int end)  {
            this.a = a;
            this.b = b;
            this.c = c;
            this.start = start;
            this.end = end;
        }

        @Override
        public void run() {
            for (int i = start ; i < end ; i++) {
                c[i] = a[i] + b[i];
            }
        }
    }

    public static class MyThread2 extends Thread {
        private int [] a , b , c ;
        private int i , p , n;

        public MyThread2(int [] a , int [] b , int [] c ,int i , int p,int n)  {
            this.a = a;
            this.b = b;
            this.c = c;
            this.i = i;
            this.p = p;
            this.n = n;
        }

        @Override
        public void run() {
            for (int pos = i ; pos < n ; pos = pos + p) {
                c[pos] = a[pos] + b[pos];
            }
        }

    }

}