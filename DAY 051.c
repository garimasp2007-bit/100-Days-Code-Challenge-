// Q101: Write a Program to take a sorted array(say nums[]) and an integer (say target) as inputs. The elements in the sorted array might be repeated. 
// You need to print the first and last occurrence of the target and print the index of first and last occurrence. Print -1, -1 if the target is not present.

// Using O(lon n) time complexity. We would use binary seach sor the same and also using function:

#include<stdio.h>

int find_first(int arr[], int n, int target){
    int low=0, high=n-1, first=-1;
    while(low<=high){
        int mid=(low+high)/2;
        if(arr[mid]==target){
            first=mid;
            high=mid-1;
        }
        else if(arr[mid]<target)
            low=mid+1;
        else 
            high=mid-1;
    }
    return first;
}

int find_last(int arr[], int n, int target){
    int low=0, high=n-1, last=-1;
    while(low<=high){
        int mid=(low+high)/2;
        if(arr[mid]==target){
            last=mid;
            low=mid+1;
        }
        else if(arr[mid]<target)
            low=mid+1;
        else 
            high=mid-1;
    }
    return last;
}

int main(){
    int n;
    printf("Enter the number of elements to enter:\n");
    scanf("%d", &n);
    int arr[n];
    printf("Enter %d elements (sorted ascending):\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    int target;
    printf("Enter target value: ");
    scanf("%d", &target);
    int first = find_first(arr, n, target);
    int last = find_last(arr, n, target);
    printf("First occurrence: %d, Last occurrence: %d\n", first, last);
    return 0;
}
