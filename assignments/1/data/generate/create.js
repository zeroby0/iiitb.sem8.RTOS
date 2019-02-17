const fs = require('fs');
const faker = require('faker');

function write(file, text) {
  return new Promise((resolve, reject) => {
    fs.writeFile(file, text, err => {
      if (err) reject(err);
      else resolve();
    });
  });
}

async function writeCard(fileName) {
	const fakeData = faker.helpers.createCard();
	await write(fileName, JSON.stringify(fakeData, null, 4))


}

for (i=0; i < 1000; i ++) {
	writeCard('../' + i + '.txt')
}

