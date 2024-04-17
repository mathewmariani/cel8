import fs from "fs";
import path from "path";
import Markdown from "markdown-it";
import Mustache from "mustache";
import { globSync } from "glob";

// mustache partials
const meta = fs.readFileSync("docs/mustache/meta.mustache", "utf8");
const header = fs.readFileSync("docs/mustache/header.mustache", "utf8");
const footer = fs.readFileSync("docs/mustache/footer.mustache", "utf8");
const page = fs.readFileSync("docs/mustache/page.mustache", "utf8");

// markdown
const md = Markdown({ html: true });

// builds a single page
function _buildPage(sections: Record<string, string>): string {
  console.log("Building page...");

  // template using mustache
  const partials = { meta: meta, header: header, footer: footer };
  return Mustache.render(page, sections, partials)
}

function _buildWebsite() {
  console.log("Building website...");

  // create output directory
  if (!fs.existsSync("website")) {
    fs.mkdirSync("website");
  }

  var sections: Record<string, string> = {};

  // glob all .md files
  const md_glob = globSync("docs/content/*.md");
  for (const file of md_glob) {
    console.log("found an .md file:", file);
    try {
      const body = fs.readFileSync(file, "utf8");
      const name = path.parse(file).name.toString();
      sections[name] = md.render(body);
    } catch (err) {
      console.error(err);
    }
  }

  // write index to output
  fs.writeFile("website/index.html", _buildPage(sections), (err) => {
    if (err) {
      console.error(err);
    }
  });

  // copy assets to output
  fs.cp("docs/assets", "website/assets", { recursive: true }, (err) => {
    if (err) {
      console.error(err);
    }
  });
}

// main.js
_buildWebsite();