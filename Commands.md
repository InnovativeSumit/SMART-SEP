# 📌 First Time Setup (Push Project to GitHub)

Follow these steps when you are pushing the project for the **first time**.

### 1️⃣ Initialize Git Repository
```bash
git init
```

### 2️⃣ Check Repository Status
```bash
git status
```

### 3️⃣ Add All Project Files
```bash
git add .
```

### 4️⃣ Commit the Files
```bash
git commit -m "Initial Smart-SEP project commit"
```

### 5️⃣ Create a Development Branch
```bash
git checkout -b dev
```

### 6️⃣ Connect GitHub Repository
```bash
git remote add origin https://github.com/InnovativeSumit/SMART-SEP.git
```

### 7️⃣ Push Code to GitHub
```bash
git push -u origin dev
```

---

# 🔄 Updating the Project (After Code Changes)

Whenever you **modify or add new code**, follow these steps.

### 1️⃣ Check Changed Files
```bash
git status
```

### 2️⃣ Add Changes
```bash
git add .
```

### 3️⃣ Commit Changes
```bash
git commit -m "Updated scheduler logic"
```

### 4️⃣ Push Changes to GitHub
```bash
git push
```

or

```bash
git push origin dev
```

---

# ⚡ Quick Workflow Summary

### First Time Setup
```bash
git init
git status
git add .
git commit -m "Initial Smart-SEP project commit"
git checkout -b dev
git remote add origin https://github.com/InnovativeSumit/SMART-SEP.git
git push -u origin dev
```

### Updating Code Later
```bash
git status
git add .
git commit -m "Updated scheduler logic"
git push
```
