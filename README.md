# Book-Recommendation-System-with-Collaborative-Filtering-using-C
System that recommends a new book by looking at a person's previous choices with the collaborative filtering method. <br>

System includes 3 subsections: <br>

<b>1.</b> Calculation of the similarity of a reader to other readers: The similarity of a reader to other readers will be calculated using the pearson coefficient.
While calculating, transactions will be made for books that both people read. Using the Pearson coefficient, the similarity of persons a and b is calculated as follows: <br>

![sim](https://user-images.githubusercontent.com/64909109/108767732-da54bd80-7567-11eb-9922-767ed3434958.PNG)
<br>
P: books read by both a and b <br>
ra,p: User a's score for book p <br>
rb,p: User b's score for book p <br>
ra: Average score of books read by user a <br>
rb: Average score of books read by user b <br>
Similarity value is between -1 and 1. <br>

<b>2.</b> Determining the k people with whom a reader is the most similar: Find the reader most similar to the reader, based on the result of the previous calculation for a given value of k without sorting. <br>

<b>3.</b> Suggesting a book : To suggest a book, make a calculation for most similar k person with equation below. On this equation, N is equal to most similar k person/s. The calculated value pred(a,p) is estimated point to a book which is not read by user. <br>
![pred](https://user-images.githubusercontent.com/64909109/108767730-d88afa00-7567-11eb-81c2-41be3491c953.PNG) <br>

<i>Input file is "RecomendationDataSet.csv".</i><br>
<B> SCREENSHOTS </b> <br>
1<br>
![1](https://user-images.githubusercontent.com/64909109/108776872-08d89580-7574-11eb-9454-c8268887447a.PNG)<br><br>
2<br>
![2](https://user-images.githubusercontent.com/64909109/108776880-0c6c1c80-7574-11eb-84e3-0f6e4e827f96.PNG)

