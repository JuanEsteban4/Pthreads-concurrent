import java.util.Random;
import java.util.concurrent.ForkJoinPool;

public class BenchmarkSorting {

    public static void main(String[] args) {
        int size = 1_000_000;
        int runs = 5;

        System.out.println("Tamaño del arreglo: " + size);
        System.out.println("Ejecuciones por algoritmo: " + runs + "\n");

        for (int i = 0; i < runs; i++) {
            System.out.println("Run #" + (i + 1));

            // Generar datos aleatorios
            Integer[] data1 = randomArray(size);
            Integer[] data2 = data1.clone();

            // QuickSort
            long start = System.currentTimeMillis();
            ForkJoinPool.commonPool().invoke(new QuickSortTask<>(0, data1.length - 1, data1));
            long quickTime = System.currentTimeMillis() - start;
            System.out.println("QuickSortTask: " + quickTime + " ms");

            // MergeSort
            start = System.currentTimeMillis();
            ForkJoinPool.commonPool().invoke(new MergeSortTask<>(data2, 0, data2.length - 1));
            long mergeTime = System.currentTimeMillis() - start;
            System.out.println("MergeSortTask: " + mergeTime + " ms");
        }
    }

    static Integer[] randomArray(int size) {
        Random rand = new Random();
        Integer[] arr = new Integer[size];
        for (int i = 0; i < size; i++) {
            arr[i] = rand.nextInt(1_000_000);
        }
        return arr;
    }
}
