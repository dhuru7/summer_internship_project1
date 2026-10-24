<!-- GitHub does not natively render D2 blocks. Export 'arka-diagram.svg' alongside your README, or click to view live in ARKA -->
[![make ER diagram for libarary man](./arka-diagram.svg)](http://localhost:5000/share/1792851833013)

<sub>[Open and edit this diagram interactively in ARKA](http://localhost:5000/share/1792851833013)</sub>

<details>
<summary>D2 Source Code</summary>

```d2
direction: right

classes: {
  user_table: { style: { fill: "#eff6ff"; stroke: "#2563eb"; stroke-width: 2; border-radius: 8; font-color: "#1e40af" } }
  order_table: { style: { fill: "#f0fdf4"; stroke: "#16a34a"; stroke-width: 2; border-radius: 8; font-color: "#14532d" } }
  product_table: { style: { fill: "#faf5ff"; stroke: "#9333ea"; stroke-width: 2; border-radius: 8; font-color: "#6b21a8" } }
  payment_table: { style: { fill: "#fff7ed"; stroke: "#ea580c"; stroke-width: 2; border-radius: 8; font-color: "#c2410c" } }
  audit_table: { style: { fill: "#fefce8"; stroke: "#ca8a04"; stroke-width: 2; border-radius: 8; font-color: "#854d0e" } }
}

authors: "authors (Book Authors)" {
  shape: sql_table
  class: product_table
  id: uuid { constraint: primary_key }
  name: varchar(255)
  biography: text
  birth_date: timestamp
  created_at: timestamp
}

categories: "categories (Book Categories)" {
  shape: sql_table
  class: order_table
  id: uuid { constraint: primary_key }
  name: varchar(100) { constraint: unique }
  description: text
  created_at: timestamp
}

books: "books (Library Books)" {
  shape: sql_table
  class: product_table
  id: uuid { constraint: primary_key }

  isbn: varchar(20) { constraint: unique }
  author_id: uuid { constraint: foreign_key }
  category_id: uuid { constraint: foreign_key }
  published_year: int
  total_copies: int
}

members: "members (Library Members)" {
  shape: sql_table
  class: user_table
  id: uuid { constraint: primary_key }
  first_name: varchar(100)
  last_name: varchar(100)
  email: varchar(255) { constraint: unique }
  phone_number: varchar(50)
  joined_date: timestamp
}

loans: "loans (Book Loans)" {
  shape: sql_table
  class: payment_table
  id: uuid { constraint: primary_key }
  book_id: uuid { constraint: foreign_key }
  member_id: uuid { constraint: foreign_key }
  loan_date: timestamp
  due_date: timestamp
  returned_date: timestamp
  status: varchar(50)
}

fines: "fines (Overdue Fines)" {
  shape: sql_table
  class: audit_table
  id: uuid { constraint: primary_key }
  loan_id: uuid { constraint: foreign_key }
  amount: decimal(10,2)
  paid: boolean
  issued_date: timestamp
}

authors.id -> books.author_id: "1..* (writes)"
categories.id -> books.category_id: "1..* (categorizes)"
books.id -> loans.book_id: "1..* (loaned in)"
members.id -> loans.member_id: "1..* (borrows)"
loans.id -> fines.loan_id: "1..1 (incurs)"
```

</details>
