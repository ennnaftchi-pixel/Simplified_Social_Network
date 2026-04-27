# Reseau Social — C Linked List Project

A command-line social network written in C using nested singly-linked lists.  
Built as a university systems-programming exercise (ISI, Tunis 2025-2026).

## Data model

```
reseau_t  ──►  user_t ──► user_t ──► user_t ──► NULL
                │
                └──► post_t ──► post_t ──► NULL
```

- **`user_t`** — outer linked list, sorted by `id_user`
- **`post_t`** — inner linked list per user, sorted by `id_post`

## Features

| Function | Description |
|---|---|
| `saisie_user` | Fill one user struct via pointer |
| `saisie_post` | Fill one post struct via pointer |
| `affiche_user` / `affiche_reseau` | Display one user or full network |
| `insere_user` / `insere_post` | Sorted insertion with duplicate check |
| `supprimer_user` | Remove a user and free all their posts |
| `total_likes` | Recursive sum of likes across all users |
| `moyenne_likes` | Average likes per user |
| `liberer_reseau` | Free all heap-allocated memory |

## Build & run

```bash
gcc -Wall -Wextra -o reseau reseau_social.c
./reseau
```

Requires a C99-compatible compiler (GCC or Clang).

## Key C concepts demonstrated

- `typedef struct` with self-referential pointer (`struct user *svt`)
- Pointer-vs-value distinction: `->` vs `.`
- `malloc` / `free` memory management
- `strcpy` for safe string assignment
- Recursive functions with `NULL` base case
- Sorted linked-list insertion algorithm

## How to upload this to GitHub (VS Code terminal)

```bash
# 1. Initialise a local git repo
git init
git add reseau_social.c README.md

# 2. First commit
git commit -m "feat: social network with nested linked lists"

# 3. Create a repo on github.com, then connect it
git remote add origin https://github.com/YOUR_USERNAME/reseau-social.git
git branch -M main
git push -u origin main
```

> Replace `YOUR_USERNAME` with your actual GitHub username.

## License

MIT — free to use for studying.
