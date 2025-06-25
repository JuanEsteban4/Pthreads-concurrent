import java.util.concurrent.*;
import java.util.Random;
import java.util.Arrays;

public class QuickSortTask<T extends Comparable<? super T>> extends RecursiveAction {

    private static final int THRESHOLD = 100;

    private int begin;
    private int end;
    private T[] array;

    public QuickSortTask(int begin, int end, T[] array) {
        this.begin = begin;
        this.end = end;
        this.array = array;
    }

    @Override
    protected void compute() {
        if (begin >= end) {
            return;
        }

        if (end - begin + 1 <= THRESHOLD) {
            insertionSort(begin, end);
            return;
        }

        T pivot = array[end];
        int leftPointer = begin;
        int rightPointer = end;

        while (leftPointer < rightPointer) {
            while (array[leftPointer].compareTo(pivot) < 0 && leftPointer < rightPointer) {
                leftPointer++;
            }

            while (array[rightPointer].compareTo(pivot) >= 0 && leftPointer < rightPointer) {
                rightPointer--;
            }

            swap(leftPointer, rightPointer);
        }

        swap(leftPointer, end);

        QuickSortTask<T> leftTask = new QuickSortTask<>(begin, leftPointer - 1, array);
        QuickSortTask<T> rightTask = new QuickSortTask<>(leftPointer + 1, end, array);
        invokeAll(leftTask, rightTask);
    }

    private void insertionSort(int left, int right) {
        for (int i = left + 1; i <= right; i++) {
            T key = array[i];
            int j = i - 1;
            while (j >= left && array[j].compareTo(key) > 0) {
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = key;
        }
    }

    private void swap(int i, int j) {
        T temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Uso: java QuickSortTask <tamaño del arreglo>");
            return;
        }

        int size = Integer.parseInt(args[0]);
        Integer[] array = new Integer[size];
        Random rand = new Random();

        for (int i = 0; i < size; i++) {
            array[i] = rand.nextInt(1000);  // números aleatorios entre 0 y 999
        }

        System.out.println("Array original:");
        System.out.println(Arrays.toString(array));

        ForkJoinPool pool = new ForkJoinPool();
        QuickSortTask<Integer> sortTask = new QuickSortTask<>(0, size - 1, array);
        pool.invoke(sortTask);

        System.out.println("Array ordenado:");
        System.out.println(Arrays.toString(array));
    }
}
