let gulp = require('gulp');
let browser = require('browser-sync');
let cp = require('child_process');

let port = process.env.SERVER_PORT || 8080;

gulp.task('build', gulp.series(function(cb) {
  let cmd = process.platform === "win32" ? "jekyll.bat" : "jekyll";
  let opts = ['build']

  let jekyll = cp.spawn(cmd, opts, { stdio: 'inherit' })
  return jekyll.on('close', cb)
}))

gulp.task('browser', gulp.series('build', function() {
  browser.init({ server: './_site', port: port })

  const list = [
    '*.css',
    '*.html',
    '_config.yml',
    '_data/*.yml',
    '_includes/*.html',
    '_includes/**/*.md',
    '_layouts/*.html',
    '_posts/*.md',
    '_sass/*scss',
  ]
  gulp.watch(list).on('change', gulp.series('build', 'browser:reload'))
}))

gulp.task('browser:reload', gulp.series('build', function() {
  browser.reload()
}))

gulp.task('default', gulp.series('browser'))