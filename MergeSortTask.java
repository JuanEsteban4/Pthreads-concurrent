import java.util.concurrent.*;
import java.util.Arrays;
import java.util.Random;

public class MergeSortTask<T extends Comparable<? super T>> extends RecursiveAction {
    private static final int THRESHOLD = 100;

    private T[] array;
    private int left;
    private int right;

    public MergeSortTask(T[] array, int left, int right) {
        this.array = array;
        this.left = left;
        this.right = right;
    }

    @Override
    protected void compute() {
        if (right - left <= THRESHOLD) {
            insertionSort(array, left, right);
        } else {
            int mid = (left + right) / 2;

            MergeSortTask<T> leftTask = new MergeSortTask<>(array, left, mid);
            MergeSortTask<T> rightTask = new MergeSortTask<>(array, mid + 1, right);
            invokeAll(leftTask, rightTask);
            merge(array, left, mid, right);
        }
    }

    private void insertionSort(T[] arr, int left, int right) {
        for (int i = left + 1; i <= right; i++) {
            T key = arr[i];
            int j = i - 1;
            while (j >= left && arr[j].compareTo(key) > 0) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

    @SuppressWarnings("unchecked")
    private void merge(T[] arr, int left, int mid, int right) {
        Object[] temp = new Object[right - left + 1];
        int i = left, j = mid + 1, k = 0;

        while (i <= mid && j <= right) {
            if (arr[i].compareTo(arr[j]) <= 0) temp[k++] = arr[i++];
            else temp[k++] = arr[j++];
        }

        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];

        for (int t = 0; t < temp.length; t++) {
            arr[left + t] = (T) temp[t];
        }
    }

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Uso: java MergeSortTask <tamaño del arreglo>");
            return;
        }

        int size = Integer.parseInt(args[0]);

        Integer[] array = new Integer[size];
        Random rand = new Random();

        for (int i = 0; i < size; i++) {
            array[i] = rand.nextInt(1000);
        }

        System.out.println("Arreglo original:");
        System.out.println(Arrays.toString(array));

        ForkJoinPool pool = new ForkJoinPool();
        MergeSortTask<Integer> task = new MergeSortTask<>(array, 0, array.length - 1);

        pool.invoke(task);

        System.out.println("\nArreglo ordenado:");
        System.out.println(Arrays.toString(array));
    }
}
